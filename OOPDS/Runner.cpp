#include <iostream>
#include <fstream>
#include <string>

using namespace std;


// Andy

// this is for parser
class Parser
{
private:
    string getCommand(string line); //extracts the instuction
    string getFirstOperand(string line);
    string getSecondOperand(string line);
    int getRegisterNumber(string reg); //converts register string to register number contoh cm R3--> 3
    int stringToInt(string str);
public:
    Instruction* parse(string line);
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

Instruction* Parser::parse(string line)
{
    string cmd = getCommand(line);
    string op1 = getFirstOperand(line);
    string op2 = getSecondOperand(line);

    int dest = getRegisterNumber(op1);
    int src = getRegisterNumber(op2);

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
        return new Output(dest);

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
    Instruction* instr = parser.parse(line);

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


// =====================================================
// MAIN FUNCTION
// =====================================================

int main()
{
    Runner runner("input.asm", "output.asm");

    runner.run();

    return 0;
}