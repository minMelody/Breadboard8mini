  LDI 0
;count_up
  OUT
  ADD $f
  JC 5
  JMP 1
;count_down
  SUB $f
  OUT
  JZ 1
  JMP 5
.org $f
  .byte 5