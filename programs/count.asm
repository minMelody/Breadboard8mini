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
.org $f
  .byte 5