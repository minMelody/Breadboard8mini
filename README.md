# Description
Emulating Ben Eater's [8 bit breadboard computer](https://youtube.com/playlist?list=PLowKtXNTBypGqImE405J2565dvjafglHU&feature=shared).<br>
The main program can load a .bin files or assembly language source file.<br>
Memory can easily be expanded and more opcodes can be added.

## Assembler
The assembler can execute at the start of the program by entering a source file. It uses a simplified version of [vasm oldstyle syntax](http://www.ibaug.de/vasm/doc/vasm.pdf#43) - used a lot in his 6502 playlist.

### Comments
Anything following `;` is ignored.

### Numbers
Write hexadecimal numbers using prefix `$`, binary numbers with prefix `%`, octal base use prefix `@`.
A digit followed by a `#` can be used to define an arbitrary base between 2 and 9.<br>
Other numbers starting with a digit are decimal.

### Directives
The `.org` and `.byte` directives are available to use.

### Labels
Labels must be defined on a new line and terminated by `:`<br>
Reference a label by writing its name in place of a memory address.

## Flags
```
Z    - Zero is set when an operation returns a zero
C    - Carry is set when an operation's result was larger than 255
HALT - Halt is set by a HLT instruction, only cleared on reset
OE   - Output Enabled is set by an OUT instruction and cleared by the next instruction
IIE  - Invalid Instruction Exception indicates the byte read by the cpu was not a valid instruction
```

## Instruction set
| Opcode | Mnemonic | Description 
|--------|----------|----------------------
| 0      | NOP      | Do nothing for one instruction cycle
| 1      | LDA      | Loads contents of a memory address to the A register
| 2      | ADD      | Adds the A register with the contents of a memory address
| 3      | SUB      | Substracts the A register with the contents of a memory address
| 4      | STA      | Stores the A register at a memory address
| 5      | LDI      | Loads a 4 bit immediate value to the A register
| 6      | JMP      | Unconditionnal jump to a memory address
| 7      | JC       | Jumps to a memory address if Carry flag is set
| 8      | JZ       | Jumps to a memory address if Zero flag is set
| E      | OUT      | Loads the A register onto the OUT register and sets the OE flag - main program then prints it
| F      | HLT      | Sets the HALT flag, used to halt program execution


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
