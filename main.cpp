#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "src/cpu.h"
#include "src/eas.h"

using namespace Breadboard8;

void read_binary(std::string path, MEM& prg)
{
	std::ifstream f(path, std::ios::binary | std::ios::ate);
	if (f.is_open())
	{
		f.seekg(0, std::ios_base::end);
		unsigned long fileSize = f.tellg();
		unsigned long address = 0x00;
		unsigned char byte = 0x00;
		while (address < prg.MAX_SIZE && address < fileSize)
		{
			f.seekg(address, std::ios_base::beg);
			f.read((char*)&byte, 1);
			prg[address] = byte;
			address++;
		}

		f.close();
		return;
	}

	std::cout << "Unable to read binary file.";
	exit(1);
}

int main(int argc, const char* argv[])
{
	std::string PROGRAM_PATH{};
	if (argc < 2)
	{
		std::cout << "Program Path: ";
		getline(std::cin, PROGRAM_PATH);
	}
	else
	{
		PROGRAM_PATH = argv[1];
	}

	MEM ram;
	if (std::filesystem::path(PROGRAM_PATH).extension().compare(".bin") == 0)
	{
		// Load program from binary file
		read_binary(PROGRAM_PATH, ram);
	}
	else
	{
		// Load program from source file
		Assembler beasm{ &ram };
		beasm.assembleProgram(PROGRAM_PATH);
	}
	CPU cpu{};
	cpu.Reset();

	// Execute until the HALT flag is set
	while (!cpu.HALT)
	{
		cpu.Execute(ram);
		if (cpu.OE) printf("%d \n", cpu.OUT);
		if (cpu.IIE) continue; // Ignore
	}

	return 0;
}