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