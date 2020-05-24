# ALCO_Project_1
Homework for Assembled Language and Computer Organization  Project One

## Project Goal
可以將輸入的一段 RISC-V 組合語言的 code 轉換成對應的 Machine Code

Input : 一段RISC-V組語的code

Output : 對應的machine code

## Requirement

:page_facing_up: 檔案需放在同個資料夾。

## Project Method
透過讀檔，將RISC-V Type種類讀入程式中。

並先將Label所在的行數先存入`vector<string> LABEL` 

先比對輸入 Name 對應的 Type ，並將其 Type 讀入剩下參數，並轉換輸出 opcode、funct7、 funct3

之後將剩下的register轉換成二進位，印出結果。


## How to Use?
Sample Input : (輸入 `exit` 表示輸入完成)
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

Sample Output :
```
0000000 10111 00010 000 00010 0110011
0000000000010 11000 000 11000 0010011
000010 10111 11000 001 000001 1100011
0000000 11011 01010 011 00000 0100011
000100 00000 00000 000 000000 1100011
0000000 00001 00010 011 00000 0100011
```

## Code Explain

```c++
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
```
`include<iostream>`  用來在Terminal輸入輸出

`include<string>`  用來使用string的 operator[]、find 等功能

`include<vector>`  儲存所有Opreator 和輸入的Instruction的空間

`include<fstream>`  讀取檔案

`include<sstream>`  分割string用

---

```c++
struct Opcode
{
	string type;
	string name;
	string opcode;
	string funct3;
	string funct7;
};
```

建立struct，並將對應的instruction放到此struct。

---

```c++
struct labelPos
{
	string name;
	string pos;
};
```

是用來儲存Label所在的行數。

---

```c++
vector<Opcode> instruction;			// 存入各種type 指令
vector<string> implement;			// 儲存使用者輸入的指令
string input;					// input handler
```

呼叫 `addinstruction()` function 來儲存 type 和對應的 name, opcode, funct3, funct7

```c++
string type;

//開檔
ifstream inFile("instruction.csv", ios::in);

//檢查檔案是否存在
if (!inFile)
{
  cerr << "File not found" << endl;
  exit(0);
}

//如果可以讀到檔案，用逗點分割Type
while (getline(inFile, type, ','))
{
  Opcode tmp;
  tmp.type = type;

  //用逗點分割Name
  getline(inFile, tmp.name, ',');

  if (type == "UJ-type")
  {
    getline(inFile, tmp.opcode, '\n');
  }
  else
  {
    getline(inFile, tmp.opcode, ',');

    //對應道不同Type有不同funct需要輸入
    if (type == "R-type" || type == "I-typeM")
    {
      getline(inFile, tmp.funct3, ',');
      getline(inFile, tmp.funct7, '\n');
    }
    else if (type == "I-type" || type == "S-type" || type == "I-typeNum" || type == "B-type")
    {
      getline(inFile, tmp.funct3, '\n');
    }
  }

  //將剛剛建立好的type存入instruction中
  instruction.push_back(tmp);
}

//關檔
inFile.close();
```

---

```c++
while (true)
{
  //從 Terminal 輸入 Instruction
  getline(cin, input);

  //若輸入exit，表示結束輸入
  if (input != "exit")
  {
    //存放輸入的指令
    implement.push_back(input);		
  }
  else
  {
    break;
  }
}
```
---

接著要判斷輸入的instruction中，是否有label

有 :arrow_right: 要記錄 Label 所在行數

```c++
string tmp;
labelPos can_find;

for (int i = 0; i < input.size(); ++i)
{
  if (input[i].find(':') != string::npos)
  {
    // 清除前一個使用的string
    tmp.clear();

    // 將找到label 的行數儲存
    for (int j = 0; input[i][j] != ':'; ++j)
    {
      tmp += input[i][j];
    }

    can_find.name = tmp;
    // 所在行數轉成 二進位
    can_find.pos = decimal_to_binary(to_string(i + 1), 12);

    label.push_back(can_find);
  }
}
```

---

接著將 instruction 指令全部 轉成 Machine code。

這邊取R-type 和 B-type 舉例

```c++
void R_type(Opcode& find, string& rd, string& rs1, string& rs2)
{
	//印出result
	cout << find.funct7 << " " << decimal_to_binary(rs2) << " " << decimal_to_binary(rs1) << " "
		<< find.funct3 << " " << decimal_to_binary(rd) << " " << find.opcode << endl;
}

void B_type(Opcode& findOp, string& rs1, string& rs2, string& offset)
{
	//label 處理
	string simm;

	//避免找不到label，整個程式crash
	bool exist = false;

	for (int i = 0; i < label.size(); ++i)
	{
		//找出對應的Label
		if (offset == label[i].name)
		{
			simm = label[i].pos;
			exist = true;
			break;
		}
	}
	
	//並且輸出
	if (exist)
	{
		for (int i = 5; i < 10; ++i)
		{
			cout << simm[i];
		}

		cout << simm[11] << " ";
	}
	else
	{
		cout << "==============Error================\n Can't Find Label" << endl;
	}

	cout << decimal_to_binary(rs2) << " " << decimal_to_binary(rs1) << " " << findOp.funct3 << " ";

	if (exist)
	{
		for (int i = 0; i < 5; ++i)
		{
			cout << simm[i];
		}

		cout << simm[10] << " ";
	}

	cout << findOp.opcode << endl;
}
```

