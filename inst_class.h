#pragma once

#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;



//涉及PC移动的函数
//--**--**--**--**--**--**--**
//特殊指令：：
//*******************************

int counter = 0;
bool flag = 1;//是否出现退出指令
//内存 类
class InterMem
{
public:
	uint8_t* Memery;
	//输入函数
	void readin()
	{
		char ch1, ch2;
		int pos = 0;
		int ins = 0;
		char read[20] = { 0 };
		while (cin >> read)
		{
			if (read[0] == '@')
			{
				pos = 0;
				int len = strlen(read);
				for (int i = 1; i < len; ++i)
				{
					if(i!=1)pos <<= 4;
					ch1 = read[i];
					if (ch1 >= '0'&&ch1 <= '9')
					{
						pos += (ch1 - '0');
					}
					else if (ch1 >= 'A'&&ch1 <= 'F')
					{
						pos += (ch1 - 'A') + 10;
					}
					
				}
				counter = pos;
			}
			else
			{
				ins = 0;
				int len = strlen(read);
				for (int i = 0; i < len; ++i)
				{
					if (i != 0)ins <<= 4;
					ch1 = read[i];
					if (ch1 >= '0'&&ch1 <= '9')
					{
						ins += (ch1 - '0');
					}
					else if (ch1 >= 'A'&&ch1 <= 'F')
					{
						ins += (ch1 - 'A') + 10;
					}
					
					
				}
				Memery[pos] = ins;
				/*cout << pos << " ";
				cout << ((Memery[pos] >>7)&1) << ((Memery[pos] >> 6) & 1) << ((Memery[pos] >> 5) & 1) << ((Memery[pos] >> 4) & 1) << ((Memery[pos] >> 3) & 1)
				<< ((Memery[pos] >> 2) & 1) << ((Memery[pos] >> 1) & 1)<< ((Memery[pos] ) & 1) << endl;*/
				pos++;
				counter++;
			}
			memset(read, 0, sizeof(read));
		}
		//cout << "4204 " << (int)Memery[4204] << endl;
		return;
		
	}
	InterMem()
	{
		Memery = new uint8_t[0x400000];
		memset(Memery, 0, 0x400000 * sizeof(uint8_t));
		//cout << "size " <<  sizeof(Memery) << endl;
		//cout << "4204 " << (int)Memery[4204] << endl;
	}
	~InterMem() {};

};


//寄存器 类
class TempStorage
{
public:
	int Register[32] = { 0 };
};
TempStorage reg;
int PC = 0;


int SignExtended(int data, int bits)//bits表示实际位数而非0-base
{
	if (data&(1 << (bits-1)))
	{
		data |= (0xffffffff >> bits << bits);
	}
	return data;
}
//-------------------------------指令集类---------------------------
class inst
{
public:
	inst() {};
	~inst() {};
	unsigned int opcode = 0;
	int func3 = 0;
	int func7 = 0;
	char elem[10] = { 0 };

	virtual void EX() = 0;
	virtual void MA(InterMem &im) = 0;
	virtual void WB() = 0;

};
//------------------------------大类-------------------------------

class R_Type:public inst
{
	
public:
	int rd = 0;
	int rs1 = 0;
	int rs2 = 0;
	R_Type() {};
	~R_Type() {};
	
};
class I_Type :public inst
{
public:
	int rd = 0;
	int rs1 = 0;
    int imm = 0;
	int shamt = 0;
	I_Type() {};
	~I_Type() {};

};
class S_Type :public inst
{public:
	int imm = 0;
	int rs1 = 0;
	int rs2 = 0;
	S_Type() {};
	~S_Type() {};
};
class U_Type :public inst
{
public:
	int rd = 0;
	int imm = 0;
	U_Type() {};
	~U_Type() {};
};

//--------------------小类-----------------------------
//--------------------S类------------------------------**
//--**--**--**--**--**--**--**都涉及PC移动
class BEQ :public S_Type
{
public:
	BEQ() {};
	~BEQ() {};
	int result = 0;
	void EX()
	{
		result = (reg.Register[rs1] == reg.Register[rs2]) ?(PC + imm ): (PC+4);
		
		return;
	}
	void MA(InterMem &im) {};
	void WB() { PC = result; };
};
class BNE :public S_Type
{
public:
	BNE() {};
	~BNE() {};
	int result = 0;
	void EX()
	{
		result = (reg.Register[rs1] == reg.Register[rs2]) ? (PC+4) : (PC + imm );
		return;
	}
	void MA(InterMem &im) {};
	void WB() { PC = result; };

};
class BLT :public S_Type
{
public:
	BLT() {};
	~BLT() {};
	int result = 0;
	void EX()
	{
		result = (reg.Register[rs1] < reg.Register[rs2]) ?
			(PC + imm ) : (PC+4);
		
		return;
	}
	void MA(InterMem &im) {};
	void WB() { PC = result; };

};
class BGE :public S_Type
{
public:
	BGE() {};
	~BGE() {};
	int result = 0;
	void EX()
	{
		result = (reg.Register[rs1] >= reg.Register[rs2]) ?
			(PC + imm ) : (PC+4);
		
		return;
	}
	void MA(InterMem &im) {};
	void WB() { PC = result; };

};
class BLTU :public S_Type
{
public:
	BLTU() {};
	~BLTU() {};
	int result = 0;
	void EX()
	{
		unsigned int tmp1 = reg.Register[rs1],
			tmp2 = reg.Register[rs2];
		result = (tmp1<tmp2) ?
			(PC + imm ) : (PC+4);
		
		return;
	}
	void MA(InterMem &im) {};
	void WB() { PC = result; };
	
};
class BGEU :public S_Type
{
public:
	BGEU() {};
	~BGEU() {};
	int result = 0;
	void EX()
	{
		unsigned int tmp1 = reg.Register[rs1],
			tmp2 = reg.Register[rs2];
		result = (tmp1 >= tmp2) ?
			(PC + imm) : (PC+4);
		return;
	}
	void MA(InterMem &im) {};
	void WB() { PC = result; };
};

//*******************************
//结束程序，返回reg.Register[10];
class SB :public S_Type
{
public:
	SB() {};
	~SB() {};
	int result = 0;
	int tmp = 0;
	void EX()
	{
		tmp = reg.Register[rs1] + imm;
		result = reg.Register[rs2] & 255;
		return;
	}
	void MA(InterMem &im)
	{
		if(tmp!=0x30004)
		im.Memery[tmp] = (result&255);
		else
		{
			flag = 0;
		}
		return;
	}
	void WB() { PC += 4; }
};
class SH :public S_Type
{
public:
	SH() {};
	~SH() {};
	int result = 0;
	int tmp = 0;
	void EX()
	{
		tmp = reg.Register[rs1] + imm;
		result = reg.Register[rs2] & 0xffff;
		return;
	}
	void MA(InterMem &im)
	{
		int tmp1 = result;
		im.Memery[tmp] = (tmp1&255);
		tmp1 >>= 8;
		im.Memery[tmp + 1] = (tmp1 & 255);
		return;
	}
	void WB() { PC += 4; };
};
class SW :public S_Type
{
public:
	SW() {};
	~SW() {};
	int result = 0;
	int tmp = 0;
	void EX()
	{
		tmp = reg.Register[rs1] + imm;
		result = reg.Register[rs2];
		return;
	}
	void MA(InterMem &im)
	{
		int tmp1 = result;
		im.Memery[tmp] = (tmp1&255);
		tmp1 >>= 8;
		im.Memery[tmp+1] = (tmp1 & 255);
		tmp1 >>= 8;
		im.Memery[tmp + 2] = (tmp1 & 255);
		tmp1 >>= 8;
		im.Memery[tmp + 3] = (tmp1 & 255);
		tmp1 >>= 8;
		
		return;
	}
	void WB() { PC += 4; };
};
//--------------------U类------------------------------**
//--**--**--**--**--**--**--**
class LUI :public U_Type
{
public:
	LUI() {};
	~LUI() {};
	int result = 0;
	void EX()
	{
		result = (imm>>12<<12);
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if(rd!=0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
//--**--**--**--**--**--**--**
class AUIPC :public U_Type
{
public:
	AUIPC() {};
	~AUIPC() {};
	int result = 0;
	void EX()
	{
		if (rd != 0)
		{
			PC += (imm << 12);
			result = PC;
		}
		
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		return;
	}
};
//--------------------I类------------------------------

class LB :public I_Type
{
public:
	LB() {};
	~LB() {};
	int result = 0;
	int tmp = 0;
	void EX()
	{
		if (rd != 0)
		tmp = reg.Register[rs1] + imm;
		return;
	}
	void MA(InterMem &im)
	{
		if (rd != 0)
		result = (int)im.Memery[tmp];
		return;
	}
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}

};
class LH :public I_Type
{
public:
	LH() {};
	~LH() {};
	int result = 0;
	int tmp = 0;
	void EX()
	{
		if (rd != 0)
		tmp = reg.Register[rs1]+ imm;
		return;
	}
	void MA(InterMem &im)
	{
		if (rd != 0)
		result = (int)((im.Memery[tmp + 1] << 8) + im.Memery[tmp]);
		return;
	}
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}

};
//***********************
class LW :public I_Type
{
public:
	LW() {};
	~LW() {};
	int result = 0;
	int tmp = 0;

	void EX()
	{
		
		tmp = reg.Register[rs1] + imm;
		return;
	}
	void MA(InterMem &im)
	{
		
		result = (int)((im.Memery[tmp + 3] << 24 )+ (im.Memery[tmp + 2] << 16) + 
			(im.Memery[tmp + 1] << 8)+ im.Memery[tmp]);
		return;
	}
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}


};
//高位补0
class LBU :public I_Type
{
public:
	LBU() {};
	~LBU() {};
	int result = 0;
	int tmp = 0;

	void EX()
	{
		if (rd != 0)
		tmp = reg.Register[rs1] + imm;
		return;
	}
	void MA(InterMem &im)
	{
		if (rd != 0)
		result = (int)((unsigned int)im.Memery[tmp]);
		return;
	}
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
//高位补0
class LHU :public I_Type
{
public:
	LHU() {};
	~LHU() {};
	int result = 0;
	int tmp = 0;
	void EX()
	{
		if (rd != 0)
		tmp = reg.Register[rs1] + imm;
		return;
	}
	void MA(InterMem &im)
	{
		if (rd != 0)
		result = (int)((unsigned int)((im.Memery[tmp + 1] << 8) + im.Memery[tmp]));
		return;
	}
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}

};
//--**--**--**--**--**--**--**都涉及PC移动
class JAL :public I_Type
{
public:
	JAL() {};
	~JAL() {};
	int result = 0;
	int tmp = 0;
	void EX()
	{
			result = PC + 4;
			PC = PC + imm;
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		return;
	}
};
class JALR :public I_Type
{
public:
	JALR() {};
	~JALR() {};
	int result = 0;
	void EX()
	{
			result = PC + 4;
			PC = ((reg.Register[rs1] + imm) >> 1 << 1);
			
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		return;
	}

};
//*****************
class ADDI :public I_Type
{
public:
	ADDI() {};
	~ADDI() {};
	int result = 0;
	void EX()
	{
		
		
			result = (reg.Register[rs1] + imm);
		
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0 )
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class SLTI :public I_Type
{
public:
	SLTI() {};
	~SLTI() {};
	int result = 0;
	void EX()
	{
		if (reg.Register[rs1] < imm)result = 1;
		else result = 0;
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;

	}
};
//
class SLTIU :public I_Type
{
public:
	SLTIU() {};
	~SLTIU() {};
	int result = 0;
	void EX()
	{
		unsigned int tmp = reg.Register[rs1];
		if (tmp <((unsigned int)imm) )result = 1;
		else result = 0;
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class ANDI :public I_Type
{
public:
	int result = 0;
	ANDI() {};
	~ANDI() {};
	void EX()
	{
		result = (reg.Register[rs1] & imm);
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class ORI :public I_Type
{
public:
	ORI() {};
	~ORI() {};
	int result = 0;

	void EX()
	{
		result = (reg.Register[rs1] | imm);
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class XORI :public I_Type
{
public:
	XORI() {};
	~XORI() {};
	int result = 0;
	void EX()
	{
		result = (reg.Register[rs1] ^ imm);
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class SLLI :public I_Type
{
public:
	SLLI() {};
	~SLLI() {};
	int result = 0;
	void EX()
	{
		int tmp = reg.Register[rs1];
		int pos = (shamt & 31u);
		result = (tmp << pos);
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class SRLI :public I_Type
{
public:
	SRLI() {};
	~SRLI() {};
	int result = 0;
	void EX()
	{
		unsigned int tmp = reg.Register[rs1];
		int pos = (shamt & 31u);
		result = (tmp >> pos);
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class SRAI :public I_Type
{
public:
	SRAI() {};
	~SRAI() {};
	int result = 0;
	void EX()
	{
		int pos = (shamt & 31u);
		int tmp0 =reg.Register[rs1];
		int tmp = (tmp0 >> pos);
		result = SignExtended(tmp, 32-pos);
		return;
	}
	void MA(InterMem &im) {};
	void WB() 
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}


};


//----------------------R类----------------------------**
class ADD :public R_Type
{
public:
	ADD() {};
	~ADD() {};

	int result = 0;
	void EX()
	{
		result = reg.Register[rs1] + reg.Register[rs2];
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}

};
class SUB :public R_Type
{
public:
	SUB() {};
	~SUB() {};
	int result = 0;
	void EX()
	{
		result = reg.Register[rs1] - reg.Register[rs2];
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class SLT :public R_Type
{
public:
	int result = 0;
	SLT() {};
	~SLT() {};
	void EX()
	{
		result = (reg.Register[rs1] < reg.Register[rs2]) ? 1 : 0;
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}

};
class SLTU :public R_Type
{
public:
	SLTU() {};
	~SLTU() {};
	int result = 0;
	
	void EX()
	{
		if (rs1 != 0)
		{
			result = (reg.Register[rs1] < reg.Register[rs2]) ? 1 : 0;

		}
		else
		{
			if (reg.Register[rs2] != 0)result = 1;
			else result = 0;
		}

		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class XOR :public R_Type
{
public:
	XOR() {};
	~XOR() {};
	int result = 0;
	void EX()
	{
		result = (reg.Register[rs1] ^ reg.Register[rs2]);
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class OR :public R_Type
{
public:
	OR() {};
	~OR() {};
	int result = 0;

	void EX()
	{
		result = (reg.Register[rs1] | reg.Register[rs2]);
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class AND :public R_Type
{
public:
	AND() {};
	~AND() {};
	int result = 0;
	void EX()
	{
		result = (reg.Register[rs1] & reg.Register[rs2]);
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class SLL :public R_Type
{
public:
	SLL() {};
	~SLL() {};
	int result = 0;
	void EX()
	{
		int tmp = reg.Register[rs2] & 31;
		result = (reg.Register[rs1] << tmp) ;
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
			reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class SRL :public R_Type
{
public:
	SRL() {};
	~SRL() {};
	int result = 0;
	void EX()
	{
		int tmp = reg.Register[rs2] & 31;
		result =(int)((unsigned int)(reg.Register[rs1]) >> tmp);
		
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};
class SRA :public R_Type
{
public:
	SRA() {};
	~SRA() {};
	int result = 0;
	void EX()
	{
		int tmp = reg.Register[rs2] & 31;
		result = (int)(reg.Register[rs1] >> tmp);
		return;
	}
	void MA(InterMem &im) {};
	void WB()
	{
		if (rd != 0)
		reg.Register[rd] = result;
		PC += 4;
		return;
	}
};

//-------------------------------Decode 过程-----------------------
class Instruction_Decode
{
public:
	Instruction_Decode() {};
	~Instruction_Decode() {};

	unsigned int instruction = 0;
	inst* R_()
	{
		unsigned int tmp = instruction;

		int opcode = (tmp & 127);
		tmp = (tmp >> 7);

		int rd = (tmp & 31);
		tmp = (tmp >> 5);

		int func3 = (tmp & 7);
		tmp = (tmp >> 3);

		int rs1 = (tmp & 31);
		tmp = (tmp >> 5);

		int rs2 = (tmp & 31);
		tmp = (tmp >> 5);

		int func7 = (tmp & 127);
		
		R_Type *p = NULL;
		
		if (func3 == 0)
		{
			if (func7 == 0)
			{
				p = new ADD;
				p->rd = rd;
				p->rs1 = rs1;
				p->rs2 = rs2;
				strcpy(p->elem, "ADD");
				
			}
			else
			{
				p = new SUB;
				p->rd = rd;
				p->rs1 = rs1;
				p->rs2 = rs2;
				strcpy(p->elem, "SUB");
				
			}
		}
		else if (func3 == 1)
		{
			p = new SLL;
			p->rd = rd;
			p->rs1 = rs1;
			p->rs2 = rs2;
			strcpy(p->elem, "SLL");
		}
		else if (func3 == 2)
		{
			p = new SLT;
			p->rd = rd;
			p->rs1 = rs1;
			p->rs2 = rs2;
			strcpy(p->elem, "SLT");
		}
		else if (func3 == 3)
		{
			p = new SLTU;
			p->rd = rd;
			p->rs1 = rs1;
			p->rs2 = rs2;
			strcpy(p->elem, "SLTU");
		}
		else if (func3 == 4)
		{
			p = new XOR;
			p->rd = rd;
			p->rs1 = rs1;
			p->rs2 = rs2;
			strcpy(p->elem, "XOR");
		}
		else if (func3 == 5)
		{
			if (func7 == 0)
			{
				p = new SRL;
				p->rd = rd;
				p->rs1 = rs1;
				p->rs2 = rs2;
				strcpy(p->elem, "SRL");
				
			}
			else
			{
				p = new SRA;
				p->rd = rd;
				p->rs1 = rs1;
				p->rs2 = rs2;
				strcpy(p->elem, "SRA");
			}
			
			
		}
		else if (func3 == 6)
		{
			p = new OR;
			p->rd = rd;
			p->rs1 = rs1;
			p->rs2 = rs2;
			strcpy(p->elem, "OR");
			
		}
		else if (func3 == 7)
		{
			p = new AND;
			p->rd = rd;
			p->rs1 = rs1;
			p->rs2 = rs2;
			strcpy(p->elem, "ANA");
		}
		return p;
	}
	inst* I_()
	{
		unsigned int tmp = instruction;

		int opcode = (tmp & 127);
		tmp = (tmp >> 7);
		I_Type* p = NULL;

		if (opcode == 3)
		{
			int rd = (tmp & 31);
			tmp >>= 5;

			int func3 = (tmp & 7);
			tmp >>= 3;

			int rs1 = (tmp & 31);
			tmp >>= 5;

			int imm1 = (tmp&(4095));
			int imm = SignExtended(imm1, 12);
			
			if (func3 == 0)
			{
				p = new LB;
				p->rd = rd;
				p->rs1 = rs1;
				p->imm = imm;
				strcpy(p->elem, "LB");
			}
			else if (func3 == 1)
			{
				p = new LH;
				p->rd = rd;
				p->rs1 = rs1;
				p->imm = imm;
				strcpy(p->elem, "LH");
			}
			else if (func3 == 2)
			{
				p = new LW;
				p->rd = rd;
				p->rs1 = rs1;
				p->imm = imm;
				strcpy(p->elem, "LW");
			}
			else if (func3 == 4)
			{
				p = new LBU;
				p->rd = rd;
				p->rs1 = rs1;
				p->imm = imm;
				strcpy(p->elem, "LBU");
			}
			else if (func3 == 5)
			{
				p = new LHU;
				p->rd = rd;
				p->rs1 = rs1;
				p->imm = imm;
				strcpy(p->elem, "LHU");
			}
			
		}
		else if (opcode == 19)
		{
			int rd = (tmp & 31);
			tmp >>= 5;

			int func3 = (tmp & 7);
			tmp >>= 3;

			int rs1 = (tmp & 31);
			tmp >>= 5;

			if (func3 == 0)
			{
				int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				p = new ADDI;
				p->imm = imm;
				p->rs1 = rs1;
				p->rd = rd;
				strcpy(p->elem, "ADDI");
			}
			else if (func3 == 2)
			{
			    int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				p = new SLTI;
				p->imm = imm;
				p->rs1 = rs1;
				p->rd = rd;
				strcpy(p->elem, "SLTI");
			}
			else if (func3 == 3)
			{
			    int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				p = new SLTIU;
				p->imm = imm;
				p->rs1 = rs1;
				p->rd = rd;
				strcpy(p->elem, "SLTUI");
			}
			else if (func3 == 4)
			{
				int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				p = new XORI;
				p->imm = imm;
				p->rs1 = rs1;
				p->rd = rd;
				strcpy(p->elem, "XORI");
			}
			else if (func3 == 6)
			{
				int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				p = new ORI;
				p->imm = imm;
				p->rs1 = rs1;
				p->rd = rd;
				strcpy(p->elem, "ORI");
			}
			else if (func3 == 7)
			{
				int imm1 = (tmp & 4095);
				int imm = SignExtended(imm1, 12);
				p = new ANDI;
				p->imm = imm;
				p->rs1 = rs1;
				p->rd = rd;
				strcpy(p->elem, "ANDI");
			}
			else
			{
				int shamt1 = (tmp & 31);
				int shamt = SignExtended(shamt1, 5);
				tmp >>= 5;
				int func7 = (tmp & 127);
				if (func3 == 1)
				{
					p = new SLLI;
					p->rd = rd;
					p->rs1 = rs1;
					p->shamt = shamt;
					strcpy(p->elem, "SLLI");
				}
				else if (func7 == 0)
				{
					p = new SRLI;
					p->rd = rd;
					p->rs1 = rs1;
					p->shamt = shamt;
					strcpy(p->elem, "SRLI");
				}
				else
				{
					p = new SRAI;
					p->rd = rd;
					p->rs1 = rs1;
					p->shamt = shamt;
					strcpy(p->elem, "SRAI");
				}

			}
			
		}
		else if (opcode == 111)
		{
		  int rd = (tmp & 31);
		  tmp >>= 5;
		  unsigned int imm1 = (tmp & 255);
		  tmp >>= 8;
		  unsigned int imm2 = (tmp & 1);
		  tmp >>= 1;
		  unsigned int imm3 = (tmp & 1023);
		  tmp >>= 10;
		  unsigned int imm4 = (tmp & 1);
		  int imm0 = 0;
		  imm0 += (imm4 << 20);
		  imm0 += (imm3 << 1);
		  imm0 += (imm2 << 11);
		  imm0 += (imm1 << 12);
		  int imm = SignExtended(imm0, 21);

		  p = new JAL;
		  p->rd = rd;
		  p->imm = imm;
		  strcpy(p->elem, "JAL");
        }
		else 
		{
		int rd = (tmp & 31);
		tmp >>= 5;
		tmp >>= 3;
		int rs1 = (tmp & 31);
		tmp >>= 5;
		int imm1 = (tmp&4095);
		int imm = SignExtended(imm1, 12);

		p = new JALR;
		p->rd = rd;
		p->rs1 = rs1;
		p->imm = imm;
		strcpy(p->elem, "JALR");
        }
		return p;
	}
	inst* U_()
	{
		
		unsigned int tmp = instruction;
		int opcode = (tmp & 127);
		tmp >>= 7;

		int rd = (tmp & 31);
		tmp >>= 5;

		int imm = (((tmp & 0xfffff) << 12)&(0xffffffff >> 12 << 12));
		
		U_Type* p= NULL;
		if (opcode == 55)
		{
			p = new LUI;
			p->rd = rd;
			p->imm = imm;
			strcpy(p->elem, "LUI");
		}
		else
		{
			p = new AUIPC;
			p->rd = rd;
			p->imm = imm;
			strcpy(p->elem, "AUIPC");
		}
		return p;
		
	};
	inst* S_()
	{
		int tmp = instruction;
		int opcode = (tmp & 127);
		tmp >>= 7;

		unsigned int imm1 = (tmp & 31);
		tmp >>= 5;

		int func3 = (tmp & 7);
		tmp >>= 3;

		int rs1 = (tmp & 31);
		tmp >>= 5;

		int rs2 = (tmp & 31);
		tmp >>= 5;

		unsigned int imm2 = (tmp & 127);

		S_Type* p = NULL;
		if (opcode == 99)
		{
			int imm0 = 0;
			imm0 += ((imm1 & 1) << 11);
			imm1 >>= 1;
			imm0 += ((imm1&15) << 1);
			imm0 += ((imm2 & 63) << 5);
			imm2 >>= 6;
			imm0 += ((imm2&1) << 12);
			imm0 = (imm0&(0xffffffff >> 1 << 1));
			int imm = SignExtended(imm0, 13);

			if (func3 == 0)
			{
				p = new BEQ;
				p->rs1 = rs1;
				p->rs2 = rs2;
				p->imm = imm;
				strcpy(p->elem, "BEQ");
			}
			else if (func3 == 1)
			{
				p = new BNE;
				p->rs1 = rs1;
				p->rs2 = rs2;
				p->imm = imm; 
				strcpy(p->elem, "BNE");
			}
			else if (func3 == 4)
			{
				p = new BLT;
				p->rs1 = rs1;
				p->rs2 = rs2;
				p->imm = imm;
				strcpy(p->elem, "BLT");
			}
			else if (func3 == 5)
			{
				p = new BGE;
				p->rs1 = rs1;
				p->rs2 = rs2;
				p->imm = imm;
				strcpy(p->elem, "BGE");
			}
			else if (func3 == 6)
			{
				p = new BLTU;
				p->rs1 = rs1;
				p->rs2 = rs2;
				p->imm = imm;
				strcpy(p->elem, "BLTU");
			}
			else if (func3 == 7)
			{
				p = new BGEU;
				p->rs1 = rs1;
				p->rs2 = rs2;
				p->imm = imm;
				strcpy(p->elem, "BGEU");
			}
			
		}
		else 
		{
			int imm0 = 0;
			imm0 += (imm1&31);
			imm0 += ((imm2&127) << 5);
			int imm = SignExtended(imm0, 12);
			if (func3 == 0)
			{
				p = new SB;
				p->imm = imm;
				p->rs1 = rs1;
				p->rs2 = rs2;
				strcpy(p->elem, "SB");
			}
			else if (func3 == 1)
			{
				p = new SH;
				p->imm = imm;
				p->rs1 = rs1;
				p->rs2 = rs2;
				strcpy(p->elem, "SH");
			}
			else 
			{
				p = new SW;
				p->imm = imm;
				p->rs1 = rs1;
				p->rs2 = rs2;
				strcpy(p->elem, "SW");
			}
		}
		return p;
	};
	inst* Decode()
	{
		int tmp =( instruction & 127);
		switch (tmp)
		{
		case 51:return R_(); break;
		case 111:case 103:case 3:case 19:return I_(); break;
		case 99:case 35:return S_(); break;
		case 55:case 23:return U_(); break;
		}
	}

};
Instruction_Decode my_dec;

//-------------------------------Fetch过程-------------------------
class Instruction_Fetch
{
public:
	Instruction_Fetch() {};
	~Instruction_Fetch() {};

	void Fecth(InterMem &im)
	{
		my_dec.instruction = (im.Memery[PC+3] << 24) + (im.Memery[PC + 2] << 16)
			+ (im.Memery[PC + 1] << 8) + im.Memery[PC];
		return;
	}

};
Instruction_Fetch Fet;