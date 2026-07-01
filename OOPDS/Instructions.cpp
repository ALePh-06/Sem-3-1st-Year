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

    void updateFlags(Flags& Flags, int result) {
    Flags.setOF(result > 127);
    Flags.setUF(result < -128);
    Flags.setZF(result == 0);
    Flags.setCF(result > 127 || result < -128);
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
    virtual void execute(Register& Register, Flags& Flags) = 0;   // pure virtual
    virtual ~Instruction() {}             // always virtual destructor
};

class IOIns : public Instruction {
protected:
    int dest;
public:
    IOIns(int d) : dest(d) {}
};

class DataTransIns : public Instruction {
protected:
    int dest;
    int src; //AKA source
public:
    DataTransIns(int d, int s) : dest(d), src(s) {}
};

class OneAddIns : public Instruction {
protected:
    int dest;
public:
    OneAddIns(int d) : dest(d) {}
};

class ArithmeticIns : public Instruction {
protected:
    int dest; // register index, e.g. 2 for R2
    int src;  // register index
public:
    ArithmeticIns(int d, int s) : dest(d), src(s) {}
    // still abstract
};

class ShiftIns : public Instruction {
protected:
    int dest;   // destination register index
    int count;  // number of positions to shift/rotate
public:
    ShiftIns(int d, int c) : dest(d), count(c) {}
    virtual void execute(CPU& cpu) = 0;
};

class Input : public IOIns {
public:
    Input(int d) : IOIns(d) {}
    void execute(Register& Register, Flags& Flags) override {
        int num;
        do {
            cout << "? ";
            cin >> num;
        } while (num < -128 || num > 127);
        
        Register.setRegister(dest, num);
    }
};

class Output : public IOIns {
public:
    Output(int d) : IOIns(d) {}
    void execute(Register& Register, Flags& Flags) override {
        int num;
        num = Register.getRegister(dest);
        cout << "R" << dest << endl;
        cout << num << endl;
    }
};

class Move : public DataTransIns {
public:
    Move(int d, int s) : DataTransIns(d, s) {}
    void execute(Register& Register, Flags& Flags) override {
        Register.setRegister(dest, src);
    }
};

class Load : public DataTransIns {
public:
    Load(int d, int s) : DataTransIns(d, s) {}
    void execute(Register& Register, Flags& Flags) override {
        Register.setRegister(dest, src);
    }
};

class Store : public DataTransIns {
public:
    Store(int d, int s) : DataTransIns(d, s) {}
    void execute(CPU& cpu, Flags& Flags) override {
        int val;
        val = Register.getRegister(dest);
        Memory.setMemory(src, val);
    }
};

class Inc : public OneAddIns {
public:
    Inc(int d) : OneAddIns(d) {}
    void execute(Register& Register, Flags& Flags) override {
        int val;
        val = Register.getRegister(dest) + 1;
        Register.setRegister(dest, val);
        Flags.updateFlags(Flags, val);
    }
};

class Dsc : public OneAddIns {
public:
    Dsc(int d) : OneAddIns(d) {}
    void execute(Register& Register, Flags& Flags) override {
        int val;
        val = Register.getRegister(dest) - 1;
        Register.setRegister(dest, val);
        Flags.updateFlags(Flags, val);
    }
};

class Add : public ArithmeticIns {
public:
    Add(int d, int s) : ArithmeticIns(d, s) {}
    void execute(Register& Register, Flags& Flags) override {
        int val;
        val = Register.getRegister(dest) + Register.getRegister(src);
        Register.setRegister(dest, val);
        Flags.updateFlags(Flags, val);
    }
};

class Sub : public ArithmeticIns {
public:
    Sub(int d, int s) : ArithmeticIns(d, s) {}
    void execute(Register& Register, Flags& Flags) override {
        int val;
        val = Register.getRegister(dest) - Register.getRegister(src);
        Register.setRegister(dest, val);
        Flags.updateFlags(Flags, val);
    }
};

class Mul : public ArithmeticIns {
public:
    Mul(int d, int s) : ArithmeticIns(d, s) {}
    void execute(Register& Register, Flags& Flags) override {
        int val;
        val = Register.getRegister(dest) * Register.getRegister(src);
        Register.setRegister(dest, val);
        Flags.updateFlags(Flags, val);
    }
};

class Div : public ArithmeticIns {
public:
    Div(int d, int s) : ArithmeticIns(d, s) {}
    void execute(Register& Register, Flags& Flags) override {
        int val;
        val = Register.getRegister(dest) / Register.getRegister(src);
        Register.setRegister(dest, val);
        Flags.updateFlags(Flags, val);
    }
};


class ShLIns : public ShiftIns {
public:
    ShLIns(int d, int c) : ShiftIns(d, c) {}

    void execute(CPU& cpu, Flags& Flags) override {
        int bits[8];
        toBinary(cpu.getRegister(dest), bits);

        for (int i = 0; i < count; i++) {
            // shift every bit left by 1, LSB (index 7) fills with 0
            for (int b = 0; b < 7; b++)
                bits[b] = bits[b + 1];
            bits[7] = 0;
        }

        signed char result = fromBinary(bits);
        cpu.setRegister(dest, result);
    }
};

class ShRIns : public ShiftIns {
public:
    ShRIns(int d, int c) : ShiftIns(d, c) {}

    void execute(CPU& cpu, Flags& Flags) override {
        int bits[8];
        toBinary(cpu.getRegister(dest), bits);

        for (int i = 0; i < count; i++) {
            // shift every bit right by 1, MSB (index 0) fills with 0
            for (int b = 7; b > 0; b--)
                bits[b] = bits[b - 1];
            bits[0] = 0;
        }

        signed char result = fromBinary(bits);
        cpu.setRegister(dest, result);
    }
};

class RoLIns : public ShiftIns {
public:
    RoLIns(int d, int c) : ShiftIns(d, c) {}

    void execute(CPU& cpu, Flags& Flags) override {
        int bits[8];
        toBinary(cpu.getRegister(dest), bits);

        for (int i = 0; i < count; i++) {
            int msb = bits[0];            // save MSB before shifting
            for (int b = 0; b < 7; b++)
                bits[b] = bits[b + 1];
            bits[7] = msb;                // wrap MSB into LSB
        }

        signed char result = fromBinary(bits);
        cpu.setRegister(dest, result);
        cpu.updateFlags(result);
    }
};

class RoRIns : public ShiftIns {
public:
    RoRIns(int d, int c) : ShiftIns(d, c) {}

    void execute(CPU& cpu, Flags& Flags) override {
        int bits[8];
        toBinary(cpu.getRegister(dest), bits);

        for (int i = 0; i < count; i++) {
            int lsb = bits[7];            // save LSB before shifting
            for (int b = 7; b > 0; b--)
                bits[b] = bits[b - 1];
            bits[0] = lsb;                // wrap LSB into MSB
        }

        signed char result = fromBinary(bits);
        cpu.setRegister(dest, result);
        cpu.updateFlags(result);
    }
};

class ResetFlag : public Instruction {
protected:
    string val;
public:
    ResetFlag(string v) : val(v) {}
    void execute(Register& reg, Flags& Flags) override {
        if (val == "CF")
            Flags.setCF(false);
        else if (val == "UF")
            Flags.setUF(false);
        else if (val == "OF")
            Flags.setOF(false);
        else if (val == "ZF")
            Flags.setZF(false);
        else {
            cout << "Unknown Flag: " << val << "\n";
            exit(EXIT_FAILURE);
        }
    }
};