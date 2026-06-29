class CPU;

class Instruction {
public:
    virtual void execute(CPU& cpu) = 0;   // pure virtual
    virtual ~Instruction() {}             // always virtual destructor
};

class ArithmeticIns : public Instruction {
protected:
    int dest; // register index, e.g. 2 for R2
    int src;  // register index
public:
    ArithmeticIns(int d, int s) : dest(d), src(s) {}
    // still abstract
};

class AddIns : public ArithmeticIns {
public:
    AddIns(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        cpu.GetRegister(dest) += cpu.GetRegister(src)
    };
};

class SubIns : public ArithmeticIns {
public:
    SubIns(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        cpu.GetRegister(dest) -= cpu.GetRegister(src)
    };
};

class MulIns : public ArithmeticIns {
public:
    MulIns(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        cpu.GetRegister(dest) *= cpu.GetRegister(src)
    }
};

class DivIns : public ArithmeticIns {
public:
    DivIns(int d, int s) : ArithmeticIns(d, s) {}
    void execute(CPU& cpu) override {
        cpu.GetRegister(dest) /= cpu.GetRegister(src)
    }
};