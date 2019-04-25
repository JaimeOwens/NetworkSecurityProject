#include <stdio.h>  
#include <string.h>
#include "table.h" 

static void CharToBit(const char input[], int output[], int bits)//把CHAR转换为INT  
{
	int i, j;
	for (j = 0; j<8; j++)
	{
		for (i = 0; i<8; i++)
		{
			output[7 * (j + 1) - i + j] = (input[j] >> i) & 1;
		}
	}
};
static void BitToChar(const int intput[], char output[], int bits)//把INT转换为CHAR  
{
	int i, j;
	for (j = 0; j<8; j++)
	{
		for (i = 0; i<8; i++)
		{
			output[j] = output[j] * 2 + intput[i + 8 * j];
		}
	}
};
static void Xor(int *INA, int *INB, int len)//异或操作  
{
	int i;
	for (i = 0; i<len; i++)
	{
		*(INA + i) = *(INA + i) ^ *(INB + i);
	}
};
static  void IP(const int input[64], int output[64], int table[64])//初始IP置换  
{
	int i;
	for (i = 0; i<64; i++)
	{
		output[i] = input[table[i] - 1];//减1操作不可少！！  
	}
};
static  void E(const int input[32], int output[48], int table[48])//E扩展  
{
	int i;
	for (i = 0; i<48; i++)
	{
		output[i] = input[table[i] - 1];
	}
};
static  void P(const int input[32], int output[32], int table[32])//P置换  
{
	int i;
	for (i = 0; i<32; i++)
	{
		output[i] = input[table[i] - 1];
	}
};
static  void IP_In(const int input[64], int output[64], int table[64])//逆IP  
{
	int i;
	for (i = 0; i<64; i++)
	{
		output[i] = input[table[i] - 1];
	}
};
static  void PC_1(const int input[64], int output[56], int table[56])//PC_1  
{
	int i;
	for (i = 0; i<56; i++)
	{
		output[i] = input[table[i] - 1];
	}
};
static  void PC_2(const int input[56], int output[48], int table[48])//PC_2  
{
	int i;
	for (i = 0; i<48; i++)
	{
		output[i] = input[table[i] - 1];
	}
};
static  void S(const int input[48], int output[32], int table[8][4][16])//S盒压缩  
{
	int i = 0;
	int j = 0;
	int INT[8];
	for (; i<48; i = i + 6)
	{
		INT[j] = table[j][(input[i] << 1) + (input[i + 5])][(input[i + 1] << 3) + (input[i + 2] << 2) + (input[i + 3] << 1) + (input[i + 4])];
		j++;
	}
	for (j = 0; j<8; j++)
	{
		for (i = 0; i<4; i++)
		{
			output[3 * (j + 1) - i + j] = (INT[j] >> i) & 1;
		}
	}
};
static void F_func(int input[32], int output[32], int subkey[48])//完成DES算法轮变换  
{
	int len = 48;
	int temp[48] = { 0 };
	int temp_1[32] = { 0 };
	E(input, temp, E_Table);
	Xor(temp, subkey, len);
	S(temp, temp_1, S_Box);
	P(temp_1, output, P_Table);
};
static void RotateL(const int input[28], int output[28], int leftCount)//秘钥循环左移  
{
	int i;
	int len = 28;
	for (i = 0; i<len; i++)
	{
		output[i] = input[(i + leftCount) % len];
	}
};
static void  subKey_fun(const int input[64], int Subkey[16][48])//子密钥生成  
{
	int loop = 1, loop_2 = 2;
	int i, j;
	int c[28], d[28];
	int pc_1[56] = { 0 };
	int pc_2[16][56] = { 0 };
	int rotatel_c[16][28] = { 0 };
	int rotatel_d[16][28] = { 0 };
	PC_1(input, pc_1, PC1_Table);
	for (i = 0; i<28; i++)
	{
		c[i] = pc_1[i];
		d[i] = pc_1[i + 28];
	}
	int leftCount = 0;
	for (i = 1; i<17; i++)
	{
		if (i == 1 || i == 2 || i == 9 || i == 16)
		{
			leftCount += loop;
			RotateL(c, rotatel_c[i - 1], leftCount);
			RotateL(d, rotatel_d[i - 1], leftCount);
		}
		else
		{
			leftCount += loop_2;
			RotateL(c, rotatel_c[i - 1], leftCount);
			RotateL(d, rotatel_d[i - 1], leftCount);
		}
	}
	for (i = 0; i<16; i++)
	{
		for (j = 0; j<28; j++)
		{
			pc_2[i][j] = rotatel_c[i][j];
			pc_2[i][j + 28] = rotatel_d[i][j];
		}
	}
	for (i = 0; i<16; i++)
	{
		PC_2(pc_2[i], Subkey[i], PC2_Table);
	}
};
static void  DES_Efun(char input[8], char key_in[8], int output[64])
{
	int Ip[64] = { 0 };//存储初始置换后的矩阵  
	int output_1[64] = { 0 };
	int subkeys[16][48];
	int chartobit[64] = { 0 };
	int key[64];
	int l[17][32], r[17][32];
	CharToBit(input, chartobit, 8);//正确,转换为64个二进制数的操作正确！  
	IP(chartobit, Ip, IP_Table);//正确，IP初始置换！  
	CharToBit(key_in, key, 8);//正确！  
	subKey_fun(key, subkeys);//正确！  
	for (int i = 0; i<32; i++)
	{
		l[0][i] = Ip[i];
		r[0][i] = Ip[32 + i];
	}
	for (int j = 1; j<16; j++)//前15轮的操作  
	{
		for (int k = 0; k<32; k++)
		{
			l[j][k] = r[j - 1][k];
		}
		F_func(r[j - 1], r[j], subkeys[j - 1]);
		Xor(r[j], l[j - 1], 32);
	}
	int t = 0;
	for (t = 0; t<32; t++)//最后一轮的操作  
	{
		r[16][t] = r[15][t];
	}
	F_func(r[15], l[16], subkeys[15]);
	Xor(l[16], l[15], 32);
	for (t = 0; t<32; t++)
	{
		output_1[t] = l[16][t];
		output_1[32 + t] = r[16][t];
	}
	IP_In(output_1, output, IPR_Table);
};
static void  DES_Dfun(int input[64], char key_in[8], char output[8])
{
	int Ip[64] = { 0 };//存储初始置换后的矩阵  
	int output_1[64] = { 0 };
	int output_2[64] = { 0 };
	int subkeys[16][48];
	int chartobit[64] = { 0 };
	int key[64];
	int l[17][32], r[17][32];
	IP(input, Ip, IP_Table);//正确，IP初始置换！  
	CharToBit(key_in, key, 8);//正确！  
	subKey_fun(key, subkeys);//正确！  
	for (int i = 0; i<32; i++)
	{
		l[0][i] = Ip[i];
		r[0][i] = Ip[32 + i];
	}
	for (int j = 1; j<16; j++)//前15轮的操作  
	{
		for (int k = 0; k<32; k++)
		{
			l[j][k] = r[j - 1][k];
		}
		F_func(r[j - 1], r[j], subkeys[16 - j]);
		Xor(r[j], l[j - 1], 32);
	}
	int t = 0;
	for (t = 0; t<32; t++)//最后一轮的操作  
	{
		r[16][t] = r[15][t];
	}
	F_func(r[15], l[16], subkeys[0]);
	Xor(l[16], l[15], 32);
	for (t = 0; t<32; t++)
	{
		output_1[t] = l[16][t];
		output_1[32 + t] = r[16][t];
	}
	IP_In(output_1, output_2, IPR_Table);
	BitToChar(output_2, output, 8);
};
int main()
{
	int output[64] = { 0 };
	char MIN[9] = { 0 };
	char MI[9] = { 0 };
	printf("请输入明文(8字节)\n");
	gets(MIN);
	printf("请输入秘钥(8字节)\n");
	gets(MI);
	DES_Efun(MIN, MI, output);
	printf("密文如下:\n");
	for (int i = 0; i<64; i++)
	{
		printf("%d", output[i]);
		if ((i + 1) % 4 == 0)
			printf("\n");
	}
	printf("\n");
	printf("解密功能\n");
	DES_Dfun(output, MI, MIN);
	printf("明文如下:\n");
	for (int i = 0; i<8; i++)
	{
		printf("%c", MIN[i]);
	}
	printf("\n\n");
	return 0;
}
