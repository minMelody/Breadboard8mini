#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "cpu.h"
#include "eas.h"

std::vector<std::string> split(std::string str, char delimiter)
{
    std::vector<std::string> vec;
    size_t pos = 0;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        std::string subst = str.substr(0, pos);
        if (subst.length() > 0) vec.push_back(subst);
        str.erase(0, pos + 1);
    }
    if (str.length() > 0) vec.push_back(str);
    return vec;
}

uint8_t Breadboard8::Assembler::parse_number(std::string str)
{
    if (str[0] == '$') return stoi(str.substr(1), nullptr, 16);
    else if (str[0] == '@') return stoi(str.substr(1), nullptr, 8);
    else if (str[0] == '%') return stoi(str.substr(1), nullptr, 2);
    else if (str[1] == '#')
    {
        std::string base = "";
        base += str[0];
        return stoi(str.substr(2), nullptr, stoi(base));
    }
    return stoi(str);
}

void Breadboard8::Assembler::writeValues(uint8_t start_adr, std::vector<std::string> values, unsigned int& counter, Breadboard8::MEM& prg)
{
    for (int i = 0; i < values.size(); i++) {
        if (start_adr + i >= prg.MAX_SIZE) break;
        uint8_t value = parse_number(values[i]);
        prg[start_adr + i] = value;
        counter += (start_adr + i == counter);
    }
}

void Breadboard8::Assembler::parse_directives(std::string directive, std::vector<std::string>  exp, unsigned int& counter, Breadboard8::MEM& prg)
{
    if (directive == ".org")
    {
        counter = parse_number(exp[0]);
    }
    else if (directive == ".byte")
    {
        writeValues(counter, exp, counter, prg);
    }
}

void Breadboard8::Assembler::writeInstruction(uint8_t opcode, uint8_t data, unsigned int& counter, Breadboard8::MEM& prg)
{
    prg[counter] = opcode + (data & 0x0f);
    counter++;
}

void Breadboard8::Assembler::assembleProgram(const std::string filePath, MEM& prg)
{
    std::ifstream f(filePath);
    if (f.is_open()) {

        unsigned int counter = 0;
        for (std::string line; getline(f, line);) {
            if (counter >= prg.MAX_SIZE) break;
            interpretLine(line, prg, counter);
        }

        f.close();
        return;
    }

    std::cout << "Unable to open file.";
    exit(1);
}

void Breadboard8::Assembler::interpretLine(std::string line, MEM& prg, unsigned int& counter)
{
    if (line == std::string("")) return;

    std::vector<std::string> ins = split(split(line, ';')[0], ' ');
    if (ins.size() < 1) return;
    std::string op = ins[0];
    std::vector<std::string> vals = std::vector<std::string>(ins.begin() + 1, ins.end());

    if (op[0] == '.') parse_directives(op, vals, counter, prg);
    else if (std::isdigit(op[0])) { // Same as a .byte directive except it only works with decimal
        writeValues(counter, ins, counter, prg);
    }
    else if (op == "NOP") writeInstruction(Breadboard8::opcodes::NOP, 0                    , counter, prg);
    else if (op == "LDA") writeInstruction(Breadboard8::opcodes::LDA, parse_number(vals[0]), counter, prg);
    else if (op == "ADD") writeInstruction(Breadboard8::opcodes::ADD, parse_number(vals[0]), counter, prg);
    else if (op == "SUB") writeInstruction(Breadboard8::opcodes::SUB, parse_number(vals[0]), counter, prg);
    else if (op == "STA") writeInstruction(Breadboard8::opcodes::STA, parse_number(vals[0]), counter, prg);
    else if (op == "LDI") writeInstruction(Breadboard8::opcodes::LDI, parse_number(vals[0]), counter, prg);
    else if (op == "JMP") writeInstruction(Breadboard8::opcodes::JMP, parse_number(vals[0]), counter, prg);
    else if (op == "JC" ) writeInstruction(Breadboard8::opcodes::JC , parse_number(vals[0]), counter, prg);
    else if (op == "JZ" ) writeInstruction(Breadboard8::opcodes::JZ , parse_number(vals[0]), counter, prg);
    else if (op == "OUT") writeInstruction(Breadboard8::opcodes::OUT, 0                    , counter, prg);
    else if (op == "HLT") writeInstruction(Breadboard8::opcodes::HLT, 0                    , counter, prg);
}