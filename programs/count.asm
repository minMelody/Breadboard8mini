; count up and down in a loop
; incrementing and decrementing by 5
  LDI 0
count_up:
  OUT
  ADD $f
  JC  count_down
  JMP count_up
count_down:
  SUB $f
  OUT
  JZ  count_up
  JMP count_down
.org $f      ; sets the assembler's program counter to 15
  .byte $05  ; writes an hexadecimal number to the current address