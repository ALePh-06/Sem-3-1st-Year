#include <iostream>
#include <cstdint>
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
    virtual ~Instruction() {}             // virtual destructor
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
    int dest; // register destination
    int src; // register source
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
    int dest;
    int src;
protected:
    int getDest() {return dest;}
    int getSrc() {return src;}
public:
    ArithmeticIns(int d, int s) : dest(d), src(s) {}
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
        } while (num < -128 || num > 127);// Repeat until input is in range
        
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

int main() {
    CPU cpu;

    return 0;
}