// Building an 8-bit breadboard computer! - Ben Eater
// https://youtube.com/playlist?list=PLowKtXNTBypGqImE405J2565dvjafglHU&feature=shared

#include "cpu.h"

// ===== MEM =====

// Initialize memory with a bunch of no-ops
Breadboard8::MEM::MEM()
{
	data.fill(0x00);
}

// Load a program in memory
template<typename... T>
Breadboard8::MEM::MEM(T... prg)
{
	int i = 0;
	for (const auto b : { prg... }) {
		data[i] = b;
		i++;
	}
}
// Load a program in memory
Breadboard8::MEM::MEM(uint8_t prog[MAX_SIZE])
{
	for (int i = 0; i < MAX_SIZE; i++) {
		data[i] = prog[i];
	}
}

Breadboard8::MEM::~MEM()
{
	/* void */
}

// Read and write byte at memory address
uint8_t& Breadboard8::MEM::operator[] (uint8_t address) {
	return data[address];
}

// ===== CPU =====

void Breadboard8::CPU::Reset()
{
	PC = A = B = OUT = 0;
	Z = C = HALT = OE = IIE = false;
	cycles = 0;
}

uint8_t Breadboard8::CPU::Adder(uint8_t A, uint8_t B, bool& C, bool& Z)
{
	unsigned int sum = A + B;
	C = (sum > 0xFF);
	Z = (sum & 0xFF) == 0;
	return sum;
}

void Breadboard8::CPU::Execute(MEM& ram)
{
	OE = IIE = false;

	// The 4 higher bits represent the opcode
	// While the 4 lowest bits represent the data (memory address, immediate value, etc.)
	IR = ram[PC];

	PC++; // Increment the program counter
	PC %= ram.MAX_SIZE; // Keep it from overflowing, Eater's program counter was only 4 bits long

	// Almost all instructions take 5 clock cycles:
	// 2 cycles FETCH sequence + 3 cycles execution.
	// The HLT instruction is quicker than the others.
	cycles = 5;

	// Instruction operations
	switch (IR >> 4)
	{
		case 0x0:	// NOP
			break;
		case 0x1:	// LDA
			A = ram[IR & 0x0F];
			break;
		case 0x2:	// ADD
			B = ram[IR & 0x0F];
			A = Adder(A, B, C, Z);
			break;
		case 0x3:	// SUB
			B = ram[IR & 0x0F];
			A = Adder(A, -B, C, Z);
			break;
		case 0x4:	// STA
			ram[IR & 0x0F] = A;
			break;
		case 0x5:	// LDI
			A = IR & 0x0F;
			break;
		case 0x6:	// JMP
			PC = IR & 0x0F;
			break;
		case 0x7:	// JC
			PC = C ? (IR & 0x0F) : PC;
			break;
		case 0x8:	// JZ
			PC = Z ? (IR & 0x0F) : PC;
			break;
		case 0xE:	// OUT
			OUT = A;
			OE = true;
			break;
		case 0xF:	// HLT
			cycles = 3;
			HALT = true;
			break;
		default:
			IIE = true;
			break;
	}
}
