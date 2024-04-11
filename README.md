# An 8bit breadboard computer emulator
Emulating Ben Eater's [8 bit breadboard computer](https://youtube.com/playlist?list=PLowKtXNTBypGqImE405J2565dvjafglHU&feature=shared).<br>
Build the project by running `./build.sh`, this will create a `./main` binary using g++.

The main program is capable to load .bin and .out binary files as well as assembly language source files, either from command line arguments or from user input.

Run the program using `./main <file path> <clock speed in hertz>`

![screenshot](https://github.com/minMelody/Breadboard8/blob/main/screenshot.png)

![screenshot1](https://github.com/minMelody/Breadboard8/blob/main/screenshot1.png)

## The CPU
These are the eleven opcodes Ben Eater built into his instruction set:

| Opcode | Mnemonic | Description                                                     | Affect Flags | Affect Registers |
|--------|----------|-----------------------------------------------------------------|--------------|------------------|
| 0      | NOP      | Do nothing for one instruction cycle                            | none         | none             |
| 1      | LDA      | Loads contents of a memory address to the A register            | none         | A                |
| 2      | ADD      | Adds the A register with the contents of a memory address       | C Z          | B A              |
| 3      | SUB      | Substracts the A register with the contents of a memory address | C Z          | B A              |
| 4      | STA      | Stores the A register at a memory address                       | none         | none             |
| 5      | LDI      | Loads a 4 bit immediate value to the A register                 | none         | A                |
| 6      | JMP      | Unconditionnal jump to a memory address                         | none         | PC               |
| 7      | JC       | Jumps to a memory address if Carry flag is set                  | none         | PC *             |
| 8      | JZ       | Jumps to a memory address if Zero flag is set                   | none         | PC *             |
| E      | OUT      | Loads the A register onto the OUT register and sets the OE flag | OE           | OUT              |
| F      | HLT      | Sets the HALT flag, used to halt program execution              | HALT         | none             |

\* only if jumping

| Flag | Description                   | Behavior
|------|-------------------------------|------------
| Z    | Zero                          | Set when an operation returns a zero
| C    | Carry                         | Set when an operation's result was larger than 255
| HALT | Halt                          | Set by a HLT instruction, only cleared on reset
| OE   | Output Enabled                | Set by an OUT instruction and cleared by the next instruction
| IIE  | Invalid Instruction Exception | Indicates the byte read by the cpu was not a valid instruction

Memory can easily be expanded and more opcodes can be added by modifying `cpu.h` and `cpu.cpp`.

## Assembler Syntax
The assembler is based off a simplified version of [Volker Barthelmann's vasm oldstyle syntax module](http://www.ibaug.de/vasm/doc/vasm.pdf#43) - used a lot in Eater's 6502 videos.<br>
As of now it implements labels as well as the `.org` and `.byte` directives.<br>
Anything following `;` is ignored and considered a comment.

### Number format
Numbers preceded by `$` are hexadecimal, while a preceding `%` designate a binary number.<br>
The assembler also accepts octal values preceded by `@`, and a digit followed by a `#` can be used to define an arbitrary base between 2 and 9.<br>
Bare numbers default to decimal.

### Labels
Labels are symbols that refer to a memory address using a user-defined mnemonic. They must be defined on a new line and terminated by `:`<br>
Refer to a label by writing its name in lieu of a memory address.

*The assembler is case insensitive, meaning a lower case symbol is identical to upper case.*

### Directives
The `.org` and `.byte` directives are available to use:

|            Usage           | Description
|----------------------------|--------------
| .org [#]adr                | Sets the base address for the following code block. Accepts an optional `#`
| .byte int1[ int2...]       | Writes the following numbers, separated by a blank space, into successive bytes of memory

An alternative to the `.byte` directive is writing decimal integers with no symbol/mnemonic preceding them:
```
.byte 8   ; Writes an 8 at the current memory location
8         ; Also writes an 8

$F        ; Will not work
.byte $F  ; But this will
```

## Example program: adding 28 and 14
A simple program adding two numbers that are located at the end of memory would look something like this:
```
  lda $e
  add $f
  out
  hlt
.org $e
  .byte 28 14
```
