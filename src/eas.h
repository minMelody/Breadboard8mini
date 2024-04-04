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
		// Assemble one line at a time.
		void parse_line(std::string line);
		// Linker.
		// Writes labels addresses in place of their mnemonics.
		void link_symbols();
	protected:
		void write_instruction(uint8_t opcode, std::string data);
		void write_values(uint8_t start_adr, std::vector<std::string> values);
		bool parse_number(std::string str, uint8_t* val);
		uint8_t counter = 0;
		MEM* prg = nullptr;	// Points to the program.
		struct Symbol
		{
			std::uint8_t address; // the symbol's address
			std::vector<std::uint8_t> references{}; // stores where the symbol is used
		};
		std::unordered_map<std::string, Symbol> symbol_table{};
	};

}