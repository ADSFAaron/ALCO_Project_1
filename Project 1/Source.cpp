#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

struct Opcode
{
	string type;
	string name;
	string opcode;
	string funct3;
	string funct7;
};

struct labelPos
{
	string name;
	string pos;
};

//global variable
vector<labelPos> label;				//�p�G���O��label�A�s���

string decimal_to_binary(string decimal, int zeroize = 5)
{
	//delete 'x' or other word which is not number
	while (true)
	{
		if (decimal[0] < '0' || decimal[0]>'9')
		{
			for (int i = 0; i < decimal.size() - 1; ++i)
			{
				decimal[i] = decimal[i + 1];
			}

			decimal[decimal.size() - 1] = '\0';
		}
		else
		{
			break;
		}
	}

	int dec = stoi(decimal);
	string returnn;

	while (dec != 0)
	{
		returnn += (dec % 2) + '0';
		dec /= 2;
	}

	//��0
	while (returnn.size() != zeroize)
	{
		returnn += '0';
	}

	//reverse binary
	int i = 0;

	while (i != (returnn.size() / 2))
	{
		swap(returnn[returnn.size() - 1 - i], returnn[0 + i]);

		++i;
	}
	/*
	cout << returnn << endl;
	system("pause");*/

	return returnn;
}

void label_detected(vector<string>& input)
{
	string tmp;
	labelPos can_find;

	for (int i = 0; i < input.size(); ++i)
	{
		if (input[i].find(':') != string::npos)
		{
			tmp.clear();

			//�N���label ������x�s
			for (int j = 0; input[i][j] != ':'; ++j)
			{
				tmp += input[i][j];
			}

			can_find.name = tmp;
			can_find.pos = decimal_to_binary(to_string(i + 1), 12);

			label.push_back(can_find);
		}
	}
}

void R_type(Opcode& find, string& rd, string& rs1, string& rs2)
{
	//�L�Xresult
	cout << find.funct7 << " " << decimal_to_binary(rs2) << " " << decimal_to_binary(rs1) << " "
		<< find.funct3 << " " << decimal_to_binary(rd) << " " << find.opcode << endl;
}

void I_type(Opcode& find, string& rd, string& rs1, string& offset)
{
	//�L�Xresult
	cout << decimal_to_binary(offset, 12) << " " << decimal_to_binary(rs1) << " "
		<< find.funct3 << " " << decimal_to_binary(rd) << " " << find.opcode << endl;
}

void S_type(Opcode& find, string& rs2, string& offset, string& rs1)
{
	//�L�Xresult
	string Roffset = decimal_to_binary(offset, 12);

	for (int i = 5; i < 12; ++i)
	{
		cout << Roffset[i];
	}

	cout << " " << decimal_to_binary(rs2) << " "
		<< decimal_to_binary(rs1) << " " << find.funct3 << " ";

	for (int i = 0; i <= 4; ++i)
	{
		cout << Roffset[i];
	}

	cout << " " << find.opcode << endl;
}

void I_type_num(Opcode& find, string& rd, string& rs1, string& imm)
{
	int num = stoi(imm);

	cout << (num < 0) ? 1 : 0;

	/*if (num < 0)
	{
		cout << "1";
	}
	else
	{
		cout << "0";
	}*/

	//�L�Xresult
	cout << decimal_to_binary(imm, 12) << " " << decimal_to_binary(rs1) << " "
		<< find.funct3 << " " << decimal_to_binary(rd) << " " << find.opcode << endl;
}

void I_type_M(Opcode& find, string& rd, string& rs1, string& imm)
{
	//�L�Xresult
	cout << find.funct7 << " " << decimal_to_binary(imm) << " " << decimal_to_binary(rs1) << " "
		<< find.funct3 << " " << decimal_to_binary(rd) << " " << find.opcode << endl;
}

void B_type(Opcode& findOp, string& rs1, string& rs2, string& offset)
{
	//label �B�z
	string simm;

	//�קK�䤣��label�A��ӵ{��crash
	bool exist = false;

	for (int i = 0; i < label.size(); ++i)
	{
		if (offset == label[i].name)
		{
			simm = label[i].pos;
			exist = true;
			break;
		}
	}

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

void UJ_type(Opcode& findOp, string& rd, string& offset)
{
	//label �B�z
	string simm;

	//�קK�䤣��label�A��ӵ{��crash
	bool exist = false;

	for (int i = 0; i < label.size(); ++i)
	{
		if (offset == label[i].name)
		{
			//���s���label�A�]���w�]b type label �u��12��
			simm = decimal_to_binary(label[i].pos, 20);
			exist = true;
			break;
		}
	}

	if (exist)
	{


		cout << simm[19] << " ";

		for (int i = 0; i < 10; ++i)
		{
			cout << simm[i] << " ";
		}

		cout << simm[10] << " ";

		for (int i = 11; i < 19; ++i)
		{
			cout << simm[i] << " ";
		}
	}
	else
	{
		cout << "==============Error================\n Can't Find Label" << endl;
	}

	cout << decimal_to_binary(rd);
	cout << findOp.opcode << endl;
}

void toMachineCode(string input, vector<Opcode>& instruction)
{
	//����input
	string nm, rd, rs1, rs2, imm, offset;

	stringstream ss(input);

	//���oname�n�P�_type
	getline(ss, nm, ' ');

	//�qvector�������instruction
	Opcode findOp;

	for (int i = 0; i < instruction.size(); ++i)
	{
		if (nm == instruction[i].name)
		{
			findOp = instruction[i];
			break;
		}
	}

	//�P�_���A����
	if (findOp.type == "R-type")
	{
		//add rd,rs1,rs2
		getline(ss, rd, ',');
		getline(ss, rs1, ',');
		getline(ss, rs2, '\n');

		R_type(findOp, rd, rs1, rs2);
	}
	else if (findOp.type == "I-type")
	{
		//lb rd,offset(rs1)
		getline(ss, rd, ',');
		getline(ss, offset, '(');
		getline(ss, rs1, ')');

		I_type(findOp, rd, rs1, offset);
	}
	else if (findOp.type == "S-type")
	{
		//sb rs2,offset(rs1)
		getline(ss, rs2, ',');
		getline(ss, offset, '(');
		getline(ss, rs1, ')');

		S_type(findOp, rs2, offset, rs1);
	}
	else if (findOp.type == "I-typeNum")
	{
		//addi rd,rs1,imm
		getline(ss, rd, ',');
		getline(ss, rs1, ',');
		getline(ss, imm, '\n');

		I_type_num(findOp, rd, rs1, imm);
	}
	else if (findOp.type == "I-typeM")
	{
		//slli rd,rs1,imm
		getline(ss, rd, ',');
		getline(ss, rs1, ',');
		getline(ss, imm, '\n');

		I_type_M(findOp, rd, rs1, imm);
	}
	else if (findOp.type == "B-type")
	{
		//BEQ rs1, rs2, offset
		getline(ss, rs1, ',');
		getline(ss, rs2, ',');
		getline(ss, offset, '\n');

		B_type(findOp, rs1, rs2, offset);
	}
	else if (findOp.type == "UJ-type")
	{
		//JAL rd, offset
		getline(ss, rd, ',');
		getline(ss, offset, '\n');

		UJ_type(findOp, rd, offset);
	}

}

//rs32i���O�W�[��vector��
void addinstruction(vector<Opcode>& instruction)
{
	string type;
	ifstream inFile("instruction.csv", ios::in);

	//�ˬd�ɮ׬O�_�s�b
	if (!inFile)
	{
		cerr << "File not found" << endl;
		system("pause");
		exit(0);
	}

	//�p�G�i�HŪ���ɮ�
	while (getline(inFile, type, ','))
	{
		Opcode tmp;
		tmp.type = type;

		getline(inFile, tmp.name, ',');

		if (type == "UJ-type")
		{
			getline(inFile, tmp.opcode, '\n');
		}
		else
		{
			getline(inFile, tmp.opcode, ',');

			if (type == "R-type" || type == "I-typeM")
			{
				getline(inFile, tmp.funct3, ',');
				getline(inFile, tmp.funct7, '\n');
			}
			else if (type == "I-type" || type == "S-type" || type == "I-typeNum" || type == "B-type")
			{
				getline(inFile, tmp.funct3, '\n');
			}
			else
			{
				cout << "Why you are here?\n";
				system("pause");
			}
		}



		instruction.push_back(tmp);

	}

	/*for (int i = 0; i < instruction.size(); ++i)
	{
		if (instruction[i].type == "R-type" || instruction[i].type == "I-typeM")
		{
			cout << instruction[i].type << "  " << instruction[i].name << "  Opcode: " << instruction[i].opcode << "  funct3: "
				<< instruction[i].funct3 << "  funct7: " << instruction[i].funct7 << endl;
		}
		else if (instruction[i].type == "I-type" || instruction[i].type == "S-type" || instruction[i].type == "I-typeNum" || instruction[i].type == "B-type")
		{
			cout << instruction[i].type << "  " << instruction[i].name << "  Opcode: " << instruction[i].opcode << "  funct3: "
				<< instruction[i].funct3 << endl;
		}
		else if (instruction[i].type == "UJ-type")
		{
			cout << instruction[i].type << "  " << instruction[i].name << "  Opcode: " << instruction[i].opcode << endl;
		}
	}*/

	inFile.close();
}

int main()
{
	vector<Opcode> instruction;			//�s�J�U��type ���O
	vector<string> implement;			//�x�s�ϥΪ̿�J�����O
	addinstruction(instruction);		//Ū�Jtype �M������opcode, etc.
	string input;						//input handler

	while (true)
	{
		getline(cin, input);

		if (input != "exit")
		{
			implement.push_back(input);		//�s���J�����O
		}
		else
		{
			break;
		}
	}

	//�P�_��J��Label
	label_detected(implement);

	/*for (int i = 0; i < label.size(); ++i)
	{
		cout << label[i].name << "  " << label[i].pos << endl;
	}*/

	for (int imp = 0; imp < implement.size(); ++imp)
	{
		toMachineCode(implement[imp], instruction);
	}


	system("pause");
	return 0;

}