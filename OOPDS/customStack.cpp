///can copy and paste this code to main C++ file if made(YEAP)

// custom stack(vector), dynamic
// this part is used to store instruction loaded from .asm
template<typename T>
class customVector {

    private:
    T* data; // array pinter in memory
    int size; 
    int capacity;
//doubles size of an array when its full, but basically move the old array to fit the new one
    void resize(){

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
        data = new T[capacity];
    }
    ~customVector(){delete[] data; }
    
    void push_back(T val) { 
        if (size ==capacity) resize();
        data[size++]= val;
    }
    //allows usage similar to an array
    T& operator[](int index){return data[index];}

    int getSize()
    {
        return size;
    }
};

//Stack
//for push and pop, 8 byte size is fixed
class customStack {
    //creats 8 slots, one per byte
private:
    signed char data[8];
    int top;

public:
customStack (): top(-1){}

void push (signed char val) 
{
    if (top >=7)
    {
        cout << "Stack overflow."<< endl;
        exit(1); //just crash the thing
    }
    data [++top] = val; //increment top and store
}
  signed char pop() {
        if (top < 0) {
            cout << "Stack underflow." << endl;
            exit(1); //also crash but on empty pop
        }
        return data[top--];
    }

    bool isEmpty() { return top == -1; }
    int getTop() { return top; }  // used as SI register value
};

//Custom Queue
//storing program instructions before execution

template <typename T>
class customQueue
{
    private:
    struct node 
    {
        T val;
        node* next;
        Node(T v) : val(v), next(nullptr) {}
    };
    node* front; //point first item to remove
    node*rear; //point last item to add
    int size;

    public:
    customQueue :front(nullptr),rear(nullptr),size(0){}

    void enqueue(T val)
    {
       Node* n = new Node(val);
        if (!rear) { front = rear = n; }
        else { rear->next = n; rear = n; }
        size++; 
    }
    T dequeue() 
    {
        if (!front) 
        { 
            cout << "Queue empty." << endl; exit(1);
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
//instruction command is a placeholder for the 3rd guy to replace
'''must replace the [placeholder] into person 3 class to be referred'''
class MovIns : public placeholder {
private:
    string dest;  // e.g. "R0"
    string src;   // e.g. "10", "R1", "[R1]"

public:
    MovIns(string d, string s) : dest(d), src(s) {}

    void execute(CPU& cpu) override {
        int destIdx = dest[1] - '0';  // "R3" → 3

        if (src[0] == '[') {
            // MOV R3, [R1] — indirect: R1 holds memory address
            int srcIdx = src[2] - '0';
            int addr = cpu.getRegister(srcIdx);
            cpu.setRegister(destIdx, cpu.getMemory(addr));

        } else if (src[0] == 'R') {
            // MOV R0, R1 — register to register
            int srcIdx = src[1] - '0';
            cpu.setRegister(destIdx, cpu.getRegister(srcIdx));

        } else {
            // MOV R0, 10 — immediate value
            cpu.setRegister(destIdx, (signed char)stoi(src));
        }
    }
};


class LoadIns : public placeholder {
private:
    string dest;  // e.g. "R1"
    string src;   // e.g. "[20]" or "[R2]"

public:
    LoadIns(string d, string s) : dest(d), src(s) {}

    void execute(CPU& cpu) override {
        int destIdx = dest[1] - '0';
        // strip the [ ] brackets
        string inner = src.substr(1, src.size() - 2);

        if (inner[0] == 'R') {
            // LOAD R1, [R2] — address is stored in R2
            int srcIdx = inner[1] - '0';
            cpu.setRegister(destIdx, cpu.getMemory(cpu.getRegister(srcIdx)));
        } else {
            // LOAD R1, [20] — address is the number directly
            cpu.setRegister(destIdx, cpu.getMemory(stoi(inner)));
        }
    }
};


class StoreIns : public placeholder {
private:
    string dest;  // e.g. "R1" or "[R2]"
    string src;   // e.g. "43" or "R1"

public:
    StoreIns(string d, string s) : dest(d), src(s) {}

    void execute(CPU& cpu) override {
        if (dest[0] == '[') {
            // STORE [R2], R1 — address is in R2, value is in R1
            int addrIdx = dest[2] - '0';
            int srcIdx = src[1] - '0';
            cpu.setMemory(cpu.getRegister(addrIdx), cpu.getRegister(srcIdx));
        } else {
            // STORE R1, 43 — value is in R1, address is 43
            int srcIdx = dest[1] - '0';
            int addr = stoi(src);
            cpu.setMemory(addr, cpu.getRegister(srcIdx));
        }
    }
};


class PushIns : public placeholder {
private:
    string reg;  // e.g. "R0"

public:
    PushIns(string r) : reg(r) {}

    void execute(CPU& cpu) override {
        int idx = reg[1] - '0';
        cpu.getStack().push(cpu.getRegister(idx));
        cpu.incSI();  // SI tracks how many items are on stack
    }
};


class PopIns : public placeholder {
private:
    string reg;  // e.g. "R0"

public:
    PopIns(string r) : reg(r) {}

    void execute(CPU& cpu) override {
        int idx = reg[1] - '0';
        cpu.setRegister(idx, cpu.getStack().pop());
        cpu.decSI();
    }
};