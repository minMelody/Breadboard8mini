#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "cpu.h"

namespace Breadboard8
{
	struct Assembler
	{
	public:
		Assembler(MEM* prg);
		// Assembles a source file then links the symbol table, results can be loaded immediately or saved as a .bin file
		void assemble_program(const std::string filePath);
		void parse_tokens(std::string line);
		void link_symbols();
	protected:
		void write_instruction(uint8_t opcode, std::string data);
		void write_values(uint8_t start_adr, std::vector<std::string> values);
		void parse_directives(std::string directive, std::vector<std::string>  exp);
		bool parse_number(std::string str, uint8_t* val);
		bool parse_number(std::string str, unsigned int* val);
		unsigned int counter = 0;
		MEM* prg = nullptr;
		struct Symbol
		{
			std::uint8_t address; // the symbol's address
			std::vector<std::uint8_t> references{}; // stores where the symbol is used
		};
		std::unordered_map<std::string, Symbol> symbol_table{};
	};

}