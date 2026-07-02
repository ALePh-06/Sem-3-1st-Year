///can copy and paste this code to main C++ file if made(YEAP)

// custom stack(vector), dynamic
// this part is used to store instruction loaded from .asm
template<typename T>
class customVector  // this class works with any data type
{

    private:
    T* data; // array pointer in memory
    int size; 
    int capacity;
//doubles size of an array when its full, but basically move the old array to fit the new one
    void resize()
    {
        capacity *= 2;
        T* newData = new T[capacity];
        for (int i=0; i<size; i++)
            newData[i] = data[i]; 
        delete[] data;
        data = newData;
    }

    public:
    customVector() :size(0),capacity(4)
    {
        data = new T[capacity]; //Constructor with size 0 and capacity 4,then allocates
    }
    ~customVector(){delete[] data; }
    
    void push_back(T val) { 
        if (size ==capacity) resize(); //checks full or not, if yes=resize then add new data
        data[size++]= val;
    }
    //allows usage similar to an array
    T& operator[](int index){return data[index];} //allows command myvector[2] along with reference if added

    int getSize()
    {
        return size;
    }
};


//Custom Queue
//storing program instructions before execution

template <typename T>
class customQueue
{
    private:
    struct Node 
    {
        T val;
        Node* next;
        Node(T v) : val(v), next(nullptr) {} //holds a value and pointer to next
    };
    Node* front; //point first item to remove
    Node* rear; //point last item to add
    int size;

    public:
    customQueue() :front(nullptr),rear(nullptr),size(0){}
    
        ~customQueue() 
        {
        while (!isEmpty()) dequeue(); //deques until empty, so memory leak didnt happen
        }
    void enqueue(T val)
    {
       Node* n = new Node(val); //makes new node, if empty then point to first node else attach node to the back
        if (!rear) { front = rear = n; } 
        else { rear->next = n; rear = n; }
        size++; 
    }
    T dequeue() //check for empty queue
    {
        if (!front) 
        { 
            cout << "Queue empty." << endl; 
            exit(1);
        }
        T val = front->val; // saving values
        Node* temp = front; //save pointer to  be deleted
        front = front->next; //move point
        if (!front) rear = nullptr; //empties queue
        delete temp;
        size--;
        return val;
    }
    bool isEmpty() { return size == 0; }
    int getSize() { return size; }
};

//MOV LOAD STORE PUSH POP
//foward declare CPU so it can be referred

class MovIns : public Instruction {
private:
    string dest;  // e.g. "R0"
    string src;   // e.g. "10", "R1", "[R1]"

public:
    MovIns(string d, string s) : dest(d), src(s) {}

    void execute(CPU& cpu) override 
    {
        int destIdx = dest[1] - '0';  // "R3" → 3, a way to convert string from dest to integer

        if (src[0] == '[') {
            // MOV R3, [R1] — indirect: R1 holds memory address
            int srcIdx = src[2] - '0';//just in case if someone added'[]' at front, as failsafe
            int addr = cpu.getReg(srcIdx);//reads r1 as memory address
            cpu.setReg(destIdx, cpu.getMem(addr)); //go to memory address, fetch value then store at the destination
            //turns out this is wrong but need more checks

        } 
        else if (src[0] == 'R') 
        {
            // MOV R0, R1 — register to register
            int srcIdx = src[1] - '0';
            cpu.setReg(destIdx, cpu.getReg(srcIdx));                //starts with 'r' then its read from source register and written to destination
        } 
        else 
        {
            // MOV R0, 10 — immediate value
            cpu.setReg(destIdx, (signed char)stoi(src));
            // if in numbers the stoi changes to integer then cast to signed char and store in destination
        }
    }
    
};


class LoadIns : public Instruction {
private:
    string dest;  // e.g. "R1"
    string src;   // e.g. "[20]" or "[R2]"

public:
    LoadIns(string d, string s) : dest(d), src(s) {}

    void execute(CPU& cpu) override {
        int destIdx = dest[1] - '0';
        // strip the [ ] brackets
        string inner = src.substr(1, src.size() - 2);
        //removes first and last characters, usually brackets
        if (inner[0] == 'R') 
        {
            // LOAD R1, [R2] — address is stored in R2
            int srcIdx = inner[1] - '0';
            int addr = cpu.getReg(srcIdx);
            cpu.setReg(destIdx, cpu.getMem(addr));
            //when inner starts with R then address stored in register then fetch value at the address
        } 
        else 
        {
            // LOAD R1, [20] — address is the number directly
            cpu.setReg(destIdx, cpu.getMem(stoi(inner)));            //if its number then convert to integer and used as memory address
        }
    }
};


class StoreIns : public Instruction 
{
private:
    string dest;  // e.g. "R1" or "[R2]"
    string src;   // e.g. "43" or "R1"

public:
    StoreIns(string d, string s) : dest(d), src(s) {}

   void execute(CPU& cpu) override 
   {
        if (dest[0] == '[') 
        {
            // STORE [R2], R1 — address is in R2, value is in R1
            int addrIdx = dest[2] - '0';
            int srcIdx = src[1] - '0';
            int addr = cpu.getReg(addrIdx);
            cpu.setMem(addr, cpu.getReg(srcIdx));
            // same thing as on load ins,but gets address in the register and then the value then write value
        } 
        else 
        {
            // STORE R1, 43 — value is in R1, address is 43
            int srcIdx = dest[1] - '0';
            int addr = stoi(src);
            cpu.setMem(addr, cpu.getReg(srcIdx));
            //same as above, should there isnt a bracket
        }
    }
};


class PushIns : public Instruction 
{
private:
    string reg;  // e.g. "R0"

public:
    PushIns(string r) : reg(r) {}
    
    void execute(CPU& cpu) override 
    {
        int idx = reg[1] - '0';//basically convert r0 to 0 and so on
        cpu.pushStack(cpu.getReg(idx));
        //read value from register and then push on stack
        // SI tracks how many items are on stack as a way to keep updating
    }
};


class PopIns : public Instruction {
private:
    string reg;  // e.g. "R0"

public:
    PopIns(string r) : reg(r) {}

    void execute(CPU& cpu) override 
    {
        int idx = reg[1] - '0';
        cpu.setReg(idx, cpu.popStack());
        //pop the top value then store to register,if empty then crash
        //updates stacks that got smaller
    }
};