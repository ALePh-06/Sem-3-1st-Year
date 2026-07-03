#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>

using namespace std;

// Alif - Converts a signed byte to an 8-element int array (MSB at index 0)
void toBinary(signed char value, int bits[8]) {
    unsigned char uval = (unsigned char)value; // reinterpret bits
    for (int i = 7; i >= 0; i--) {
        bits[i] = uval & 1;
        uval >>= 1;
    }
}

// Alif - Converts an 8-element int array (MSB at index 0) back to signed byte
signed char fromBinary(int bits[8]) {
    unsigned char result = 0;
    for (int i = 0; i < 8; i++) {
        result = (result << 1) | (bits[i] & 1);
    }
    return (signed char)result;
}

class customStack {
private:
    signed char data[8]; //creats 8 slots, one per byte
    int SI;
    //refers to stack index register
public:
customStack (): SI(-1){}
//same as stack where -1 is made empty
int getSI () {
        return SI;} //returns SI value for cpu to recognize stack depth

void push (int8_t val) 
{
    if (SI >=7)// checks if the stack is full or not, if yes the increment SI and store the data
    {
        cout << "Stack overflow."<< endl;
        exit(1); //just crash the thing
    }
    data [++SI] = val; //increment top and store
}
signed char pop() {
    if (SI < 0)//checks if empty and if true then SI is decremented, value stays but unreachable 
    {
        cout << "Stack underflow." << endl;
        exit(1); //also crash but on empty pop
    }
    return data[SI--];
    }

    bool isEmpty() { return SI == -1; }//empties the SI that was decremented
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

class Registers {

    private:
    int8_t R[8];

    public:
    Registers () 
    {
        for ( int i = 0; i < 8; i++ )
            R[i] = 0;
    }

    int8_t getRegister (int pos) {
        return R[pos];
    }

    void setRegister (int pos, int value) {
        R[pos] = value;
    } 
};

class Memory {
    private:
    int8_t M[64];

    public:
    Memory ()
    {
        for ( int i = 0; i < 64; i++)
        M[i] = 0;
    }

    int8_t getMemory ( int pos ) {
        return M[pos];
    }

    void setMemory ( int pos, int value ) {
        M[pos] = value;
    }

};

class Flags {
    private:
    bool UF = false, OF = false, ZF = false, CF = false;

    public:

    bool getUF () {
        return UF;
    }

    bool getOF () {
        return OF;
    }

    bool getZF () {
        return ZF;
    }

    bool getCF () {
        return CF;
    }

    void setUF ( bool b ) {
        UF =  b;
    }

     void setOF ( bool b ) {
        OF =  b;
    }

    void setZF ( bool b ) {
        ZF =  b;
    }

    void setCF ( bool b ) {
        CF =  b;
    }

    void updateFlags(int result) {
    setOF(result > 127);
    setUF(result < -128);
    setZF(result == 0);
    setCF(result > 127 || result < -128);
}
};

class ProgramCounter {
    private:
    uint8_t PC;

    public:

    ProgramCounter() {
        PC = 0;
    }

    uint8_t getPC () {
        return PC;
    }

    void IncPC () {
        PC++;
    }
};


class StackIndex {
    private:
    uint8_t SI;

    public:

    StackIndex () {
        SI = 0;
    }

    uint8_t getSI () {
        return SI;
    }

    void IncSI () {
        SI++;
    }

    void DecSI () {
        SI--;
    }
};

class CPU {
    private:
    Registers reg;
    Memory mem;
    Flags flags;
    ProgramCounter PC;
    customStack CS;
    
    public:

    CPU () {};
    // Registers focus
    void setReg( int pos, int value) { reg.setRegister(pos, value); };
    int8_t getReg(int pos) { return reg.getRegister(pos); };

    // Memory focus
    void setMem (int pos, int value) { mem.setMemory(pos, value); };
    int8_t getMem( int pos ) { return mem.getMemory(pos); };

    // PC focus
    int8_t getPC() { return PC.getPC(); };
    void incrementPC() { PC.IncPC(); };
    
    // Flags focus
    bool getUF () { return flags.getUF(); };
    bool getOF () { return flags.getOF(); };
    bool getZF () { return flags.getZF(); };
    bool getCF () { return flags.getCF(); };
    void setUF(bool b) { flags.setUF(b); }
    void setOF(bool b) { flags.setOF(b); }
    void setZF(bool b) { flags.setZF(b); }
    void setCF(bool b) { flags.setCF(b); }
    void updateFlags(int result) { flags.updateFlags(result); }

    // Stack Focus
    void pushStack(int8_t val)   { CS.push(val); }
    int8_t popStack()            { return CS.pop(); }
    int getSI()                  { return CS.getSI(); }
};

//ALif
class Instruction {
public:
    virtual void execute(CPU& cpu) = 0;   // pure virtual
    virtual ~Instruction() {}             // always virtual destructor
};

//ALif
class IOIns : public Instruction {
private:
    int dest;
protected:
    int getDest() {return dest;}
public:
    IOIns(int d) : dest(d) {}
};

//ALif
class DataTransIns : public Instruction {
private:
    int dest;
    int src; //AKA source
protected:
    int getDest() {return dest;}
    int getSrc() {return src;}
public:
    DataTransIns(int d, int s) : dest(d), src(s) {}
};

//ALif
class IncrIns : public Instruction {
private:
    int dest;
protected:
    int getDest() {return dest;}
public:
    IncrIns(int d) : dest(d) {}
};

//ALif
class ArithmeticIns : public Instruction {
private:
    int dest; // register index, e.g. 2 for R2
    int src;  // register index
protected:
    int getDest() {return dest;}
    int getSrc() {return src;}
public:
    ArithmeticIns(int d, int s) : dest(d), src(s) {}
    // still abstract
};

//ALif
class ShiftIns : public Instruction {
private:
    int dest;   // destination register index
    int count;  // number of positions to shift/rotate
protected:
    int getDest() {return dest;}
    int getCount() {return count;}
public:
    ShiftIns(int d, int c) : dest(d), count(c) {}
};

//ALif
class StackIns : public Instruction {
private:
    int dest;
protected:
    int getDest() { return dest; }
public:
    StackIns(int d) : dest(d) {}
};

//ALif
class Input : public IOIns {
public:
    Input(int d) : IOIns(d) {}
    void execute(CPU& cpu) override {
        int num;
        do {
            cout << "? ";
            cin >> num;
        } while (num < -128 || num > 127);
        
        cpu.setReg(getDest(), num);
    }
};

//ALif
class Display : public IOIns {
public:
    Display(int d) : IOIns(d) {}
    void execute(CPU& cpu) override {
        int num;
        num = cpu.getReg(getDest());
        cout << "R" << getDest() << endl;
        cout << num << endl;
    }
};

//ALif
class Move : public DataTransIns {
public:
    Move(int d, int s) : DataTransIns(d, s) {}
    void execute(CPU& cpu) override {
        cpu.setReg(getDest(), getSrc());
    }
};

//ALif
class Load : public DataTransIns {
public:
    Load(int d, int s) : DataTransIns(d, s) {}
    void execute(CPU& cpu) override {
        cpu.setReg(getDest(), getSrc());
    }
};

//ALif
class Store : public DataTransIns {
public:
    Store(int d, int s) : DataTransIns(d, s) {}
    void execute(CPU& cpu) override {
        int val;
        val = cpu.getReg(getDest());
        cpu.setMem(getSrc(), val);
    }
};

//ALif
class Inc : public IncrIns {
public:
    Inc(int d) : IncrIns(d) {}
    void execute(CPU& cpu) override {
        int val;
        val = cpu.getReg(getDest()) + 1;
        cpu.setReg(getDest(), val);
        cpu.updateFlags(val);
    }
};

//ALif
class Dsc : public IncrIns {
public:
    Dsc(int d) : IncrIns(d) {}
    void execute(CPU& cpu) override {
        int val;
        val = cpu.getReg(getDest()) - 1;
        cpu.setReg(getDest(), val);
        cpu.updateFlags(val);
    }
};

//ALif
class Add : public ArithmeticIns {
public:
    Add(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        int val;
        val = cpu.getReg(getDest()) + cpu.getReg(getSrc());
        cpu.setReg(getDest(), val);
        cpu.updateFlags(val);
    }
};

//ALif
class Sub : public ArithmeticIns {
public:
    Sub(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        int val;
        val = cpu.getReg(getDest()) - cpu.getReg(getSrc());
        cpu.setReg(getDest(), val);
        cpu.updateFlags(val);
    }
};

//ALif
class Mul : public ArithmeticIns {
public:
    Mul(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        int val;
        val = cpu.getReg(getDest()) * cpu.getReg(getSrc());
        cpu.setReg(getDest(), val);
        cpu.updateFlags(val);
    }
};

//ALif
class Div : public ArithmeticIns {
public:
    Div(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        int val;
        val = cpu.getReg(getDest()) / cpu.getReg(getSrc());
        cpu.setReg(getDest(), val);
        cpu.updateFlags(val);
    }
};

//ALif
class ShLIns : public ShiftIns {
public:
    ShLIns(int d, int c) : ShiftIns(d, c) {}

    void execute(CPU& cpu) override {
        int bits[8];
        toBinary(cpu.getReg(getDest()), bits);

        for (int i = 0; i < getCount(); i++) {
            // shift every bit left by 1, LSB (index 7) fills with 0
            for (int b = 0; b < 7; b++)
                bits[b] = bits[b + 1];
            bits[7] = 0;
        }

        signed char result = fromBinary(bits);
        cpu.setReg(getDest(), result);
    }
};

//ALif
class ShRIns : public ShiftIns {
public:
    ShRIns(int d, int c) : ShiftIns(d, c) {}

    void execute(CPU& cpu) override {
        int bits[8];
        toBinary(cpu.getReg(getDest()), bits);

        for (int i = 0; i < getCount(); i++) {
            // shift every bit right by 1, MSB (index 0) fills with 0
            for (int b = 7; b > 0; b--)
                bits[b] = bits[b - 1];
            bits[0] = 0;
        }

        signed char result = fromBinary(bits);
        cpu.setReg(getDest(), result);
    }
};

//ALif
class RoLIns : public ShiftIns {
public:
    RoLIns(int d, int c) : ShiftIns(d, c) {}

    void execute(CPU& cpu) override {
        int bits[8];
        toBinary(cpu.getReg(getDest()), bits);

        for (int i = 0; i < getCount(); i++) {
            int msb = bits[0];            // save MSB before shifting
            for (int b = 0; b < 7; b++)
                bits[b] = bits[b + 1];
            bits[7] = msb;                // wrap MSB into LSB
        }

        signed char result = fromBinary(bits);
        cpu.setReg(getDest(), result);
    }
};

//ALif
class RoRIns : public ShiftIns {
public:
    RoRIns(int d, int c) : ShiftIns(d, c) {}

    void execute(CPU& cpu) override {
        int bits[8];
        toBinary(cpu.getReg(getDest()), bits);

        for (int i = 0; i < getCount(); i++) {
            int lsb = bits[7];            // save LSB before shifting
            for (int b = 7; b > 0; b--)
                bits[b] = bits[b - 1];
            bits[0] = lsb;                // wrap LSB into MSB
        }

        signed char result = fromBinary(bits);
        cpu.setReg(getDest(), result);
    }
};

//ALif
class Push : public StackIns {
public:
    Push(int d) : StackIns(d) {}
    void execute(CPU& cpu) override {
        cpu.pushStack(cpu.getReg(getDest()));
    }
};

//ALif
class Pop : public StackIns {
public:
    Pop(int d) : StackIns(d) {}
    void execute(CPU& cpu) override {
        cpu.setReg(getDest(), cpu.popStack());
    }
};

//ALif
class ResetFlag : public Instruction {
private:
    string val;
protected:
    string getVal() {return val;}
public:
    ResetFlag(string v) : val(v) {}
    void execute(CPU& cpu) override {
        if (getVal() == "CF")
            cpu.setCF(false);
        else if (getVal() == "UF")
            cpu.setUF(false);
        else if (getVal() == "OF")
            cpu.setOF(false);
        else if (getVal() == "ZF")
            cpu.setZF(false);
        else {
            cout << "Unknown Flag: " << val << "\n";
            exit(EXIT_FAILURE);
        }
    }
};

class Helper
{
private:
    int getRegisterNumber(string operand);
    int stringToInt(string str);

public:
    bool isRegister(string operand);
    bool isImmediate(string operand);
    int resolveValueReg(string operand, CPU& cpu);
    int resolveAddressReg(string operand, CPU& cpu);
};

int Helper::getRegisterNumber(string operand)
{
    for (int i = 0; i < operand.length(); i++)
    {
        if (operand[i] >= '0' && operand[i] <= '7')
            return operand[i] - '0';
    }

    return -1;
}

int Helper::stringToInt(string str)
{
    int num = 0;

    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
            num = num * 10 + (str[i] - '0');
    }

    return num;
}

bool Helper::isRegister(string operand)
{
    if (operand.length() == 2 && operand[0] == 'R')
    {
        if (operand[1] >= '0' && operand[1] <= '7')
            return true;
    }

    return false;
}

bool Helper::isImmediate(string operand)
{
    bool hasDigit = false;

    for (int i = 0; i < operand.length(); i++)
    {
        if (operand[i] >= '0' && operand[i] <= '9')
            hasDigit = true;
        else if (operand[i] != ' ')
            return false;
    }

    return hasDigit;
}

int Helper::resolveValueReg(string operand, CPU& cpu)
{
    if (operand.find('[') != -1)
    {
        int reg = getRegisterNumber(operand);
        int address = cpu.getReg(reg);
        return cpu.getMem(address);
    }

    if (operand.find('R') != -1)
    {
        int reg = getRegisterNumber(operand);
        return cpu.getReg(reg);
    }

    return stringToInt(operand);
}

int Helper::resolveAddressReg(string operand, CPU& cpu)
{
    if (operand.find('R') != -1)
    {
        int reg = getRegisterNumber(operand);
        return cpu.getReg(reg);
    }

    return stringToInt(operand);
}

// Andy

// this is for parser
class Parser
{
private:
    Helper helper;

    string getCommand(string line);//extracts the instuction
    string getFirstOperand(string line);
    string getSecondOperand(string line);
    int getRegisterNumber(string reg);//converts register string to register number contoh cm R3--> 3
    int stringToInt(string str);

public:
    Instruction* parse(string line, CPU& cpu);
};
string Parser::getCommand(string line)
{
    int spacePos = line.find(' ');

    if (spacePos == -1)
        return line;

    return line.substr(0, spacePos);
}

string Parser::getFirstOperand(string line)
{
    int spacePos = line.find(' ');
    int commaPos = line.find(',');

    if (spacePos == -1)
        return "";

    if (commaPos == -1)
        return line.substr(spacePos + 1);

    return line.substr(spacePos + 1, commaPos - spacePos - 1);
}

string Parser::getSecondOperand(string line)
{
    int commaPos = line.find(',');

    if (commaPos == -1)
        return "";

    return line.substr(commaPos + 1);
}

int Parser::getRegisterNumber(string reg)
{
    for (int i = 0; i < reg.length(); i++)
    {
        if (reg[i] >= '0' && reg[i] <= '7')
            return reg[i] - '0';
    }

    return -1;
}

int Parser::stringToInt(string str)
{
    int num = 0;
    int i = 0;

    while (i < str.length() && str[i] == ' ')
        i++;

    for (; i < str.length(); i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
            num = num * 10 + (str[i] - '0');
    }

    return num;
}

Instruction* Parser::parse(string line, CPU& cpu)
{
    string cmd = getCommand(line);
    string op1 = getFirstOperand(line);
    string op2 = getSecondOperand(line);

    int dest = getRegisterNumber(op1);
    int src = getRegisterNumber(op2);

    if (cmd == "MOV")
    {
        int value = helper.resolveValueReg(op2, cpu);
        return new Move(dest, value);
    }

    if (cmd == "LOAD")
    {
        int address = helper.resolveAddressReg(op2, cpu);
        int value = cpu.getMem(address);
        return new Load(dest, value);
    }

    if (cmd == "STORE")
    {
        int value = helper.resolveValueReg(op1, cpu);
        int address = helper.resolveAddressReg(op2, cpu);

        return new Store(value, address);
    }


    if (cmd == "ADD")
        return new Add(dest, src);

    if (cmd == "SUB")
        return new Sub(dest, src);

    if (cmd == "MUL")
        return new Mul(dest, src);

    if (cmd == "DIV")
        return new Div(dest, src);

    if (cmd == "INC")
        return new Inc(dest);

    if (cmd == "DEC")
        return new Dsc(dest);

    if (cmd == "DISPLAY")
        return new Display(dest);

    if (cmd == "INPUT")
        return new Input(dest);

    return nullptr;
}





class Runner
{
private:
    string inputFile;
    string outputFile;

    customQueue<string> instructionQueue;
    CPU cpu;
    Parser parser;

    bool isEmptyLine(string line);
    void writeFinalOutput(ofstream& out);

public:
    Runner(string input, string output);

    void run();
    void readFile();
    void executeProgram();
    void printOutput();
    void saveOutput();
    void decodeAndExecute(string line);
};

Runner::Runner(string input, string output)
{
    inputFile = input;
    outputFile = output;
}

void Runner::run()
{
    readFile();
    executeProgram();
    printOutput();
    saveOutput();
}

bool Runner::isEmptyLine(string line)
{
    return line.length() == 0;
}

void Runner::readFile()
{
    ifstream infile;
    string line;

    infile.open(inputFile.c_str());

    if (!infile)
    {
        cout << "Error: Cannot open input file." << endl;
        return;
    }

    while (getline(infile, line))
    {
        if (!isEmptyLine(line))
        {
            instructionQueue.enqueue(line);
        }
    }

    infile.close();
}

void Runner::executeProgram()
{
    while (!instructionQueue.isEmpty())
    {
        string line = instructionQueue.dequeue();

        decodeAndExecute(line);

        cpu.incrementPC();    // 
    }
}

void Runner::decodeAndExecute(string line)
{
    Instruction* instr = parser.parse(line, cpu);

    if (instr == nullptr)
    {
        cout << "Invalid instruction: " << line << endl;
        exit(1);
    }

    instr->execute(cpu);
    delete instr;
}

void Runner::printOutput()
{
    cout << endl;
    cout << "Program finished." << endl;
    cout << "Instructions executed: ";
    cout << (int)cpu.getPC() << endl;
}

void Runner::saveOutput()
{
    ofstream out;

    out.open(outputFile.c_str());

    if (!out)
    {
        cout << "Error: Cannot create output file." << endl;
        return;
    }

    writeFinalOutput(out);

    out.close();
}

void Runner::writeFinalOutput(ofstream& out)
{
    out << "#Begin#" << endl;

    out << "#Registers#";
    for (int i = 0; i < 8; i++)
        out << (int)cpu.getReg(i) << "#";
    out << endl;

    out << "#Flags#OF#" << cpu.getOF();
    out << "#UF#" << cpu.getUF();
    out << "#CF#" << cpu.getCF();
    out << "#ZF#" << cpu.getZF() << "#" << endl;

    out << "#PC#" << (int)cpu.getPC() << "#" << endl;

    out << "#Memory#" << endl;
    for (int i = 0; i < 64; i++)
    {
        out << "#" << (int)cpu.getMem(i);

        if ((i + 1) % 8 == 0)
            out << "#" << endl;
    }

    out << "#End#" << endl;
}

int main()
{
    string inputFile;
    string outputFile;

    cout << "Enter input assembly file (.asm): ";
    cin >> inputFile;

    cout << "Enter output result file: ";
    cin >> outputFile;

    Runner runner(inputFile, outputFile);
    runner.run();

    cout << "\nProgram executed successfully." << endl;
    cout << "Output written to: " << outputFile << endl;

    return 0;
}