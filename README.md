# Description
Emulating Ben Eater's [8 bit breadboard computer](https://youtube.com/playlist?list=PLowKtXNTBypGqImE405J2565dvjafglHU&feature=shared).<br>
The main program can load a .bin files or assembly language source file.

## Assembler
The assembler can execute at the start of the program by entering a source file. It uses a simplified version of [vasm oldstyle syntax](http://www.ibaug.de/vasm/doc/vasm.pdf#43) - used a lot in the 6502 playlist.

### Comments
Anything following `;` is ignored.

### Numbers
Write hexadecimal numbers using prefix `$`, binary numbers with prefix `%`, octal base use prefix `@`.
A digit followed by a `#` can be used to define an arbitrary base between 2 and 9.

### Directives
The `.org` and `.byte` directives are available to use.

## Flags
```
Z    - Zero is set when an operation returns a zero
C    - Carry is set when an operation's result was larger than 255
HALT - Halt is set after a HLT instruction, only cleared on reset
OE   - Output Enabled is set right after a OUT instruction and cleared during the following clock cycle
IIE  - Invalid Instruction Exception indicates the cpu read an instruction not found in its instruction set
```

## Instruction set
| Opcode | Mnemonic | Description 
|--------|----------|----------------------
| 0      | NOP      | Do nothing for one instruction cycle
| 1      | LDA      | Loads contents of a memory address to the A register
| 2      | ADD      | Adds the A register with the contents of a memory address
| 3      | SUB      | Substracts the A register with the contents of a memory address
| 4      | STA      | Stores the A register at a memory address
| 6      | JMP      | Unconditionnal jump to a memory address
| 7      | JC       | Jumps to a memory address if Carry flag is set
| 8      | JZ       | Jumps to a memory address if Zero flag is set
| E      | OUT      | Loads the A register onto the OUT register and sets the OE flag - main program then prints it
| F      | HLT      | Sets the HALT flag, used to halt program execution

Opcodes 9 through D are free to add your own.

### Example program
A simple program adding two numbers - 28 and 14 - that are located at the end of memory would look something like this:
```
LDA $e
ADD $f
OUT
HLT
.org $e
.byte 28 14
```
