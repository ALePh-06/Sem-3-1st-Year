#include <iostream>
#include <vector>
#include <cstdint>
using namespace std;

// Converts a signed byte to an 8-element int array (MSB at index 0)
void toBinary(signed char value, int bits[8]) {
    unsigned char uval = (unsigned char)value; // reinterpret bits
    for (int i = 7; i >= 0; i--) {
        bits[i] = uval & 1;
        uval >>= 1;
    }
}

// Converts an 8-element int array (MSB at index 0) back to signed byte
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

    void updateFlags(Flags& flags, int result) {
    flags.setOF(result > 127);
    flags.setUF(result < -128);
    flags.setZF(result == 0);
    flags.setCF(result > 127 || result < -128);
}
};

class CPU;

class Register {

    private:
    int8_t R[8];

    public:
    Register () 
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


class Instruction {
public:
    virtual void execute(Register& reg, Memory& mem, Flags& flags) = 0;   // pure virtual
    virtual ~Instruction() {}             // always virtual destructor
};

class IOIns : public Instruction {
private:
    int dest;
protected:
    int getDest() {return dest;}
public:
    IOIns(int d) : dest(d) {}
};

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

class IncrIns : public Instruction {
private:
    int dest;
protected:
    int getDest() {return dest;}
public:
    IncrIns(int d) : dest(d) {}
};

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

class Input : public IOIns {
public:
    Input(int d) : IOIns(d) {}
    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int num;
        do {
            cout << "? ";
            cin >> num;
        } while (num < -128 || num > 127);
        
        reg.setRegister(getDest(), num);
    }
};

class Display : public IOIns {
public:
    Display(int d) : IOIns(d) {}
    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int num;
        num = reg.getRegister(getDest());
        cout << "R" << getDest() << endl;
        cout << num << endl;
    }
};

class Move : public DataTransIns {
public:
    Move(int d, int s) : DataTransIns(d, s) {}
    void execute(Register& reg, Memory& mem, Flags& flags) override {
        reg.setRegister(getDest(), getSrc());
    }
};

class Load : public DataTransIns {
public:
    Load(int d, int s) : DataTransIns(d, s) {}
    void execute(Register& reg, Memory& mem, Flags& flags) override {
        reg.setRegister(getDest(), getSrc());
    }
};

class Store : public DataTransIns {
public:
    Store(int d, int s) : DataTransIns(d, s) {}
    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int val;
        val = reg.getRegister(getDest());
        mem.setMemory(getSrc(), val);
    }
};

class Inc : public IncrIns {
public:
    Inc(int d) : IncrIns(d) {}
    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int val;
        val = reg.getRegister(getDest()) + 1;
        reg.setRegister(getDest(), val);
        flags.updateFlags(flags, val);
    }
};

class Dsc : public IncrIns {
public:
    Dsc(int d) : IncrIns(d) {}
    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int val;
        val = reg.getRegister(getDest()) - 1;
        reg.setRegister(getDest(), val);
        flags.updateFlags(flags, val);
    }
};

class Add : public ArithmeticIns {
public:
    Add(int d, int s) : ArithmeticIns(d, s) {}
    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int val;
        val = reg.getRegister(getDest()) + reg.getRegister(getSrc());
        reg.setRegister(getDest(), val);
        flags.updateFlags(flags, val);
    }
};

class Sub : public ArithmeticIns {
public:
    Sub(int d, int s) : ArithmeticIns(d, s) {}
    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int val;
        val = reg.getRegister(getDest()) - reg.getRegister(getSrc());
        reg.setRegister(getDest(), val);
        flags.updateFlags(flags, val);
    }
};

class Mul : public ArithmeticIns {
public:
    Mul(int d, int s) : ArithmeticIns(d, s) {}
    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int val;
        val = reg.getRegister(getDest()) * reg.getRegister(getSrc());
        reg.setRegister(getDest(), val);
        flags.updateFlags(flags, val);
    }
};

class Div : public ArithmeticIns {
public:
    Div(int d, int s) : ArithmeticIns(d, s) {}
    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int val;
        val = reg.getRegister(getDest()) / reg.getRegister(getSrc());
        reg.setRegister(getDest(), val);
        flags.updateFlags(flags, val);
    }
};


class ShLIns : public ShiftIns {
public:
    ShLIns(int d, int c) : ShiftIns(d, c) {}

    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int bits[8];
        toBinary(reg.getRegister(getDest()), bits);

        for (int i = 0; i < getCount(); i++) {
            // shift every bit left by 1, LSB (index 7) fills with 0
            for (int b = 0; b < 7; b++)
                bits[b] = bits[b + 1];
            bits[7] = 0;
        }

        signed char result = fromBinary(bits);
        reg.setRegister(getDest(), result);
    }
};

class ShRIns : public ShiftIns {
public:
    ShRIns(int d, int c) : ShiftIns(d, c) {}

    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int bits[8];
        toBinary(reg.getRegister(getDest()), bits);

        for (int i = 0; i < getCount(); i++) {
            // shift every bit right by 1, MSB (index 0) fills with 0
            for (int b = 7; b > 0; b--)
                bits[b] = bits[b - 1];
            bits[0] = 0;
        }

        signed char result = fromBinary(bits);
        reg.setRegister(getDest(), result);
    }
};

class RoLIns : public ShiftIns {
public:
    RoLIns(int d, int c) : ShiftIns(d, c) {}

    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int bits[8];
        toBinary(reg.getRegister(getDest()), bits);

        for (int i = 0; i < getCount(); i++) {
            int msb = bits[0];            // save MSB before shifting
            for (int b = 0; b < 7; b++)
                bits[b] = bits[b + 1];
            bits[7] = msb;                // wrap MSB into LSB
        }

        signed char result = fromBinary(bits);
        reg.setRegister(getDest(), result);
    }
};

class RoRIns : public ShiftIns {
public:
    RoRIns(int d, int c) : ShiftIns(d, c) {}

    void execute(Register& reg, Memory& mem, Flags& flags) override {
        int bits[8];
        toBinary(reg.getRegister(getDest()), bits);

        for (int i = 0; i < getCount(); i++) {
            int lsb = bits[7];            // save LSB before shifting
            for (int b = 7; b > 0; b--)
                bits[b] = bits[b - 1];
            bits[0] = lsb;                // wrap LSB into MSB
        }

        signed char result = fromBinary(bits);
        reg.setRegister(getDest(), result);
    }
};

class ResetFlag : public Instruction {
private:
    string val;
protected:
    string getVal() {return val;}
public:
    ResetFlag(string v) : val(v) {}
    void execute(Register& reg, Memory& mem, Flags& flags) override {
        if (getVal() == "CF")
            flags.setCF(false);
        else if (getVal() == "UF")
            flags.setUF(false);
        else if (getVal() == "OF")
            flags.setOF(false);
        else if (getVal() == "ZF")
            flags.setZF(false);
        else {
            cout << "Unknown Flag: " << val << "\n";
            exit(EXIT_FAILURE);
        }
    }
};
