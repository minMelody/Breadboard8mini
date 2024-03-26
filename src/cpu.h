#pragma once
#include <array>

namespace Breadboard8
{
	struct opcodes
	{
	public:
		static constexpr uint8_t
			NOP = 0x00,
			LDA = 0x10,
			ADD = 0x20,
			SUB = 0x30,
			STA = 0x40,
			LDI = 0x50,
			JMP = 0x60,
			JC  = 0x70,
			JZ  = 0x80,
			//  = 0x90,
			//  = 0xa0,
			//  = 0xb0,
			//  = 0xc0,
			//  = 0xd0,
			OUT = 0xe0,
			HLT = 0xf0;
	};

	struct MEM
	{
	public:
		// Memory size can easily be upgraded by changing this value
		static constexpr unsigned int MAX_SIZE{ 16 };

		MEM();
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
	protected:
		uint8_t Adder(uint8_t A, uint8_t B, bool& C, bool& Z);
	};
}