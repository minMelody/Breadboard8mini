// Building an 8-bit breadboard computer! - Ben Eater
// https://youtube.com/playlist?list=PLowKtXNTBypGqImE405J2565dvjafglHU&feature=shared

#include "cpu.h"

// Initialize memory with a bunch of no-ops
Breadboard8::MEM::MEM()
{
	data.fill(opcodes::NOP);
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


void Breadboard8::CPU::Reset()
{
	PC = A = B = OUT = 0;
	Z = C = HALT = OE = IIE = false;
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
	PC %= ram.MAX_SIZE; // Keeps it from overflowing, Eater's program counter was only 4 bits long

	switch (IR & 0xF0)
	{
		case opcodes::NOP:
			break;
		case opcodes::LDA:
			A = ram[IR & 0x0F];
			break;
		case opcodes::ADD:
			B = ram[IR & 0x0F];
			A = Adder(A, B, C, Z);
			break;
		case opcodes::SUB:
			B = ram[IR & 0x0F];
			A = Adder(A, -B, C, Z);
			break;
		case opcodes::STA:
			ram[IR & 0x0F] = A;
			break;
		case opcodes::LDI:
			A = IR & 0x0F;
			break;
		case opcodes::JMP:
			PC = IR & 0x0F;
			break;
		case opcodes::JC:
			PC = C ? (IR & 0x0F) : PC;
			break;
		case opcodes::JZ:
			PC = Z ? (IR & 0x0F) : PC;
			break;
		case opcodes::OUT:
			OUT = A;
			OE = true;
			break;
		case opcodes::HLT:
			HALT = true;
			break;
		default:
			IIE = true;
			break;
	}
}
