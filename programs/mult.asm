  LDA 14
  SUB 12
  JC 6
  LDA 13
  OUT
  HLT
  STA 14
  LDA 13
  ADD 15
  STA 13
  JMP 0
  NOP
  .byte 1 0 ; product
  .byte 7 8 ; operands