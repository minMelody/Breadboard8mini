top:
  LDA product
  ADD y
  STA product
  LDA x
  SUB decrement
  STA x
  JZ  end
  JMP top
end:
  LDA product
  OUT
  HLT
decrement:
  1
x:
  7
y:
  6
product:
  0