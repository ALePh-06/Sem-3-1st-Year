#include <iostream>
#include <vector>
using namespace std;

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

    void resetFlags () {
        UF = false, OF = false, ZF = false, CF = false;
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

class CPU () {

}
int main(){

    return 0;
}