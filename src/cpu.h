#pragma once
#include <array>
#include <string>
#include <unordered_map>

namespace Breadboard8
{
	struct MEM
	{
	public:
		// A list of all valid mnemonics in lowercase and the corresponding opcodes.
		const std::unordered_map<std::string, uint8_t> opcodes {
			{"nop", 0x00},
			{"lda", 0x10},
			{"add", 0x20},
			{"sub", 0x30},
			{"sta", 0x40},
			{"ldi", 0x50},
			{"jmp", 0x60},
			{"jc" , 0x70},
			{"jz" , 0x80},
			{"", 0x90},
			{"", 0xA0},
			{"", 0xB0},
			{"", 0xC0},
			{"", 0xD0},
			{"out", 0xE0},
			{"hlt", 0xF0},
		};

		// Memory size can easily be upgraded by changing this value.
		static constexpr unsigned int MAX_SIZE{ 16 };

		// Initialize memory with a bunch of no-ops.
		MEM();
		// Load a program in memory.
		MEM(uint8_t prog[MAX_SIZE]);
		template<typename... T>
		// Load a program in memory.
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
	protected:
		uint8_t Adder(uint8_t A, uint8_t B, bool& C, bool& Z);
	};
}