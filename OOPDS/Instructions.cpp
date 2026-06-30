#include <iostream>
using namespace std;

class CPU;

class Instruction {
public:
    virtual void execute(CPU& cpu) = 0;   // pure virtual
    virtual ~Instruction() {}             // always virtual destructor
};

class IOIns : public Instruction {
protected:
    int dest;
public:
    IOIns(int d) : dest(d) {}
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

class Input : public IOIns {
public:
    Input(int d) : IOIns(d) {}
    void execute(CPU& cpu) override {
        int num;
        do {
            cout << "? ";
            cin >> num;
        } while (num < -128 || num > 127)
        
        cpu.InsertRegister(dest, num);
    }
};

class Output : public IOIns {
public:
    Output(int d) : IOIns(d) {}
    void execute(CPU& cpu) override {
        int num;
        num = cpu.GetRegister(dest);
        cout << "R" << dest << endl;
        cout << num << endl;
    }
};

class Inc : public OneAddIns {
public:
    Inc(int d) : OneAddIns(d) {}
    void execute(CPU& cpu) override {
        cpu.GetRegister(dest) += 1;
    }
};

class Inc : public OneAddIns {
public:
    Inc(int d) : OneAddIns(d) {}
    void execute(CPU& cpu) override {
        cpu.GetRegister(dest) -= 1;
    }
};

class Add : public ArithmeticIns {
public:
    Add(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        cpu.GetRegister(dest) += cpu.GetRegister(src);
    }
};

class Sub : public ArithmeticIns {
public:
    Sub(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        cpu.GetRegister(dest) -= cpu.GetRegister(src);
    }
};

class Mul : public ArithmeticIns {
public:
    Mul(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        cpu.GetRegister(dest) *= cpu.GetRegister(src);
    }
};

class Div : public ArithmeticIns {
public:
    Div(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        cpu.GetRegister(dest) /= cpu.GetRegister(src);
    }
};


class ShiftR : public ArithmeticIns {
    public:
    ShiftR(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        cpu.GetRegister(dest) *= 2*src;
    }
};

class ShiftL : public ArithmeticIns {
    public:
    ShiftL(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        cpu.GetRegister(dest) /= 2*src;
    }
};