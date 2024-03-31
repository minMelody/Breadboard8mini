#include <iostream>
#include <vector>
#include <string>
#include <fstream>
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

void Breadboard8::Assembler::writeValues(uint8_t start_adr, std::vector<std::string> values)
{
    for (int i = 0; i < values.size(); i++) {
        if (start_adr + i >= prg->MAX_SIZE) break;
        uint8_t value = parse_number(values[i]);
        (*prg)[start_adr + i] = value;
        counter += (start_adr + i == counter);
    }
}

void Breadboard8::Assembler::parse_directives(std::string directive, std::vector<std::string> exp)
{
    if (directive == ".org")
    {
        counter = parse_number(exp[0]);
    }
    else if (directive == ".byte")
    {
        writeValues(counter, exp);
    }
}

void Breadboard8::Assembler::writeInstruction(uint8_t opcode, uint8_t data)
{
    (*prg)[counter] = opcode + (data & 0x0f);
    counter++;
}

Breadboard8::Assembler::Assembler(MEM* prg)
{
    this->counter = 0;
    this->prg = prg;
}

void Breadboard8::Assembler::assembleProgram(const std::string filePath)
{
    this->counter = 0;
    std::ifstream f(filePath);
    if (f.is_open()) {

        unsigned int counter = 0;
        for (std::string line; getline(f, line);) {
            if (counter >= prg->MAX_SIZE) break;
            interpretLine(line);
        }

        f.close();
        return;
    }

    std::cout << "Unable to open file.";
    exit(1);
}

void Breadboard8::Assembler::interpretLine(std::string line)
{
    if (line == std::string("")) return;

    std::vector<std::string> ins = split(split(line, ';')[0], ' ');
    if (ins.size() < 1) return;
    std::string op = ins[0];
    std::vector<std::string> vals = std::vector<std::string>(ins.begin() + 1, ins.end());
    if (vals.size() == 0) vals = {"0"};

    if (op[0] == '.') parse_directives(op, vals);
    else if (std::isdigit(op[0])) writeValues(counter, ins); // Same as a .byte directive except it only works with decimal
    else writeInstruction(prg->opcodes.at(op), parse_number(vals[0]));
}
