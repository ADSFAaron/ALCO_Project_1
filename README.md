# ALCO_Project_1
Homework for Assembled Language and Computer Organization  Project One

## Project Goal
可以將輸入的一段 RISC-V 組合語言的 code 轉換成對應的 Machine Code

Input : 一段RISC-V組語的code

Output : 對應的machine code

## Project Method
透過讀檔，將RISC-V Type種類讀入程式中。
先比對輸入 Name 對應的 Type ，並將其 Type 讀入剩下參數。

## How to Use?
Input : 
```
add x2,x2,x23
addi x24,x24,2
bne x24,x23,L2
sd x27, 0(x10)
beq x0,x0,L1
L2:
sd x1, 0(x2)
L1:
exit
```

Output :
```
0000000 10111 00010 000 00010 0110011
0000000000010 11000 000 11000 0010011
000010 10111 11000 001 000001 1100011
0000000 11011 01010 011 00000 0100011
000100 00000 00000 000 000000 1100011
0000000 00001 00010 011 00000 0100011
```
