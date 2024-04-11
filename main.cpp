#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

#include "src/clock.h"
#include "src/cpu.h"
#include "src/eas.h"

using namespace Breadboard8;

void read_binary(std::string path, MEM& prg)
{
	std::ifstream f(path, std::ios::binary | std::ios::ate);
	if (f.is_open())
	{
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

// Some programs will not output anything for a while if clock speed is set too low
// This background worker prints a spinning bar to indicate the cpu is working
void busy_indicator()
{
	const char* indicator = "/-\\|";
	int index = 0;
	while (1)
	{
		printf("%c\r", indicator[index]);
		index = (index + 1) % 4;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void dump_memory(MEM ram)
{
	for (int i = 0; i < ram.MAX_SIZE; i++)
	{
		printf("%02X ", ram[i]);
	}
	std::cout << "\n\n";
}

int main(int argc, const char* argv[])
{
	std::string PROGRAM_PATH = (argc > 1) ? argv[1] : "";
	unsigned int EMU_SPEED = (argc > 2) ? std::stoi(argv[2]) : 0;

	if (PROGRAM_PATH == "")
	{
		std::cout << "Program Path: ";
		getline(std::cin, PROGRAM_PATH);
	}
	while (EMU_SPEED == 0)
	{
		std::string buffer{};
		std::cout << "Clock speed (Hz): ";
		getline(std::cin, buffer);
		EMU_SPEED = std::stoi(buffer);
	}

	MEM ram;
	if (std::filesystem::path(PROGRAM_PATH).extension().compare(".bin") == 0
	 || std::filesystem::path(PROGRAM_PATH).extension().compare(".out") == 0)
	{
		// Load program from binary file
		read_binary(PROGRAM_PATH, ram);
	}
	else
	{
		// Load program from source file
		Assembler beasm{ &ram };
		beasm.assemble_program(PROGRAM_PATH);
	}
	
	dump_memory(ram);

	Clock clk = Clock(EMU_SPEED);
	CPU cpu{};
	cpu.Reset();

	std::thread busy_thread (busy_indicator);
	busy_thread.detach();

	// Execute until the HALT flag is set
	while (!cpu.HALT)
	{
		if (cpu.cycles == 0)
		{
			cpu.Execute(ram);
			if (cpu.OE) printf("%d \n", cpu.OUT);
			if (cpu.IIE) continue; // Ignore
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / clk.frequency));
		cpu.cycles--;
	}

	busy_thread.~thread();
	printf("\rProgram execution complete!");
	
	return 0;
}