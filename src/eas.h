#pragma once
#include <string>
#include "cpu.h"

namespace Breadboard8
{
	struct Assembler
	{
	public:
		Assembler(MEM* prg);
		void assembleProgram(const std::string filePath);
		void interpretLine(std::string line);
	protected:
		void writeInstruction(uint8_t opcode, uint8_t data);
		void writeValues(uint8_t start_adr, std::vector<std::string> values);
		void parse_directives(std::string directive, std::vector<std::string>  exp);
		uint8_t parse_number(std::string str);
		unsigned int counter = 0;
		MEM* prg = nullptr;
	};

}