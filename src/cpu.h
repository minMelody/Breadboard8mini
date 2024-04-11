#pragma once
#include <cstdint>
#include <array>
#include <string>
#include <unordered_map>

namespace Breadboard8
{
	struct MEM
	{
	public:
		// A list of all valid mnemonics in lowercase and the corresponding opcodes.
		// Opcodes 0x9 through 0xD can easily be added here.
		// Instruction operation is defined inside the CPU::Execute() implementation in 'cpu.cpp'.
		const std::unordered_map<std::string, uint8_t> opcodes {
			{"nop", 0x0},
			{"lda", 0x1},
			{"add", 0x2},
			{"sub", 0x3},
			{"sta", 0x4},
			{"ldi", 0x5},
			{"jmp", 0x6},
			{"jc" , 0x7},
			{"jz" , 0x8},
			{"", 0x9},
			{"", 0xA},
			{"", 0xB},
			{"", 0xC},
			{"", 0xD},
			{"out", 0xE},
			{"hlt", 0xF},
		};

		// Memory size can easily be upgraded by changing this value.
		static constexpr unsigned int MAX_SIZE{ 16 };

		// Initialize memory with a bunch of no-ops.
		MEM();
		// Load a program in memory.
		MEM(uint8_t prog[MAX_SIZE]);
		template<typename... T>
		MEM(T... prg);

		~MEM();

		uint8_t& operator[] (uint8_t address);
	protected:
		std::array<uint8_t, MAX_SIZE> data;
	};

	struct CPU
	{
		public:
			uint8_t PC;		// Program Counter
			uint8_t IR;		// Instruction Register
			uint8_t A, B, OUT;	// Registers

			bool Z, C;	// CPU Flags
			bool HALT;
			bool OE;	// Output Enable
			bool IIE;	// Invalid Instruction Exception

			void Reset();
			void Execute(MEM& ram);
			
			int cycles; // Numbers of cycles left to execute current instruction.
	protected:
		uint8_t Adder(uint8_t A, uint8_t B, bool& C, bool& Z);
	};
}