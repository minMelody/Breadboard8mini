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

bool Breadboard8::Assembler::parse_number(std::string str, uint8_t* val)
{
    unsigned int val_max = *val;
    bool result = parse_number(str, &val_max);
    if (result == true) *val = val_max;
    return result;
}

bool Breadboard8::Assembler::parse_number(std::string str, unsigned int* val)
{
    uint8_t temp = *val;
    if (str[0] == '$') *val = stoi(str.substr(1), nullptr, 16);     // hexadecimal
    else if (str[0] == '@') *val = stoi(str.substr(1), nullptr, 8); // octal
    else if (str[0] == '%') *val = stoi(str.substr(1), nullptr, 2); // binary
    else if (str[1] == '#') // arbitrary base
    {
        std::string base = "";
        base += str[0];
        *val = stoi(str.substr(2), nullptr, stoi(base));
    }
    else if (isdigit(str[0])) *val = stoi(str); // decimal

    return (temp != *val);
}

void Breadboard8::Assembler::write_values(uint8_t start_adr, std::vector<std::string> values)
{
    for (int i = 0; i < values.size(); i++) {
        if (start_adr + i >= prg->MAX_SIZE) break;
        uint8_t value;
        parse_number(values[i], &value);
        (*prg)[start_adr + i] = value;
        counter += (start_adr + i == counter);
    }
}

void Breadboard8::Assembler::parse_directives(std::string directive, std::vector<std::string> exp)
{
    if (directive == ".org")
    {
        std::string adr = exp[0];
        if (adr[0] == '#') adr = adr.substr(1);
        parse_number(adr, &counter);
    }
    else if (directive == ".byte")
    {
        write_values(counter, exp);
    }
}

void Breadboard8::Assembler::write_instruction(uint8_t opcode, std::string data)
{
    // In case a label is referenced here, the memory address will be written with an invalid lower nibble (4 bits) until the linker function is called.
    uint8_t val;
    if (parse_number(data, &val) == false)
    {
        if (symbol_table.contains(data)) symbol_table.at(data).references.push_back(counter);
        else
        {
            Symbol symbol{};
            symbol.references.push_back(counter); // register this address to the symbol table
            symbol_table.insert_or_assign(data, symbol);
        }
    }
    (*prg)[counter] = opcode + (val & 0x0f);
    counter++;
}

Breadboard8::Assembler::Assembler(MEM* prg)
{
    this->counter = 0;
    this->prg = prg;
}

void Breadboard8::Assembler::assemble_program(const std::string filePath)
{
    this->counter = 0;
    this->symbol_table.clear();
    std::ifstream f(filePath);
    if (f.is_open()) {

        unsigned int counter = 0;
        for (std::string line; getline(f, line);) {
            if (counter >= prg->MAX_SIZE) break;
            parse_tokens(line);
        }

        f.close();
        link_symbols();

        return;
    }

    std::cout << "Unable to open file.";
    exit(1);
}

void Breadboard8::Assembler::parse_tokens(std::string line)
{
    if (line == std::string("")) return;
    if (line[0] == ';') return;

    std::vector<std::string> ins = split(split(line, ';')[0], ' ');
    if (ins.size() < 1) return;
    std::string op = ins[0];
    std::vector<std::string> vals = std::vector<std::string>(ins.begin() + 1, ins.end());
    if (vals.size() == 0) vals = {"0"};

    if (prg->opcodes.contains(op)) write_instruction(prg->opcodes.at(op), vals[0]);
    else if (op.back() == ':')
    {
        op.pop_back();
        if (symbol_table.contains(op)) symbol_table.at(op).address = counter;
        else
        {
            Symbol symbol{};
            symbol.address = counter;
            symbol_table.insert_or_assign(op, symbol);
        }
    }
    else if (op[0] == '.') parse_directives(op, vals);
    else if (std::isdigit(op[0])) write_values(counter, ins); // Same as a .byte directive except it only works with decimal
}

#define LOG_SYMBOLTABLE

void Breadboard8::Assembler::link_symbols()
{
    for (std::pair<std::string, Symbol> symbol : symbol_table)
    {
        for (uint8_t adr : symbol.second.references)
        {
            (*prg)[adr] &= 0xF0;
            (*prg)[adr] |= symbol.second.address & 0x0F;
        }
    }
}
