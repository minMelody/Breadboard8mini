#pragma once
#include <string>
#include <map>

namespace Breadboard8
{
	struct Assembler
	{
	public:
		void assembleProgram(const std::string filePath, MEM& prg);
		void interpretLine(std::string line, MEM& prg, unsigned int& counter);
	protected:
		void writeInstruction(uint8_t opcode, uint8_t data, unsigned int& counter, Breadboard8::MEM& prg);
		void writeValues(uint8_t start_adr, std::vector<std::string> values, unsigned int& counter, Breadboard8::MEM& prg);
		void parse_directives(std::string directive, std::vector<std::string>  exp, unsigned int& counter, Breadboard8::MEM& prg);
		uint8_t parse_number(std::string str);
	};

}