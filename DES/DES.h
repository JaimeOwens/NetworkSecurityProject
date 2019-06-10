#include <cstring>
#include <cstdio>  
#include "table.h" 

class DES{
private:
	char key[9];
public:
	void Xor(int *INA, int *INB, int len);
	void subKey_fun(const int input[64], int Subkey[16][48]);
	void RotateL(const int input[28], int output[28], int leftCount);
	void F_func(int input[32], int output[32], int subkey[48]);
	void BitToChar(const int intput[], char output[], int bits);
	void CharToBit(const char input[], int output[], int bits);
	void IP(const int input[64], int output[64], int table[64]);
	void E(const int input[32], int output[48], int table[48]);
	void P(const int input[32], int output[32], int table[32]);
	void IP_In(const int input[64], int output[64], int table[64]);
	void PC_1(const int input[64], int output[56], int table[56]);
	void PC_2(const int input[56], int output[48], int table[48]);
	void S(const int input[48], int output[32], int table[8][4][16]);
	void DES_Efun(char input[8], char key_in[8], int output[64]);
	void DES_Dfun(int input[64], char output[8]);
	void SetKey(char *buffer);
	char *GetKey();
	void DisplayOutput(int *output);
	void DisplayMessage(char *message);
};

void DES::CharToBit(const char input[], int output[], int bits){
	int i, j;
	for (j = 0; j<8; j++){
		for (i = 0; i<8; i++){
			output[7 * (j + 1) - i + j] = (input[j] >> i) & 1;
		}
	}
}

void DES::BitToChar(const int intput[], char output[], int bits){
	int i, j;
	for (j = 0; j<8; j++){
		for (i = 0; i<8; i++){
			output[j] = output[j] * 2 + intput[i + 8 * j];
		}
	}
}

void DES::Xor(int *INA, int *INB, int len){
	int i;
	for (i = 0; i<len; i++){
		*(INA + i) = *(INA + i) ^ *(INB + i);
	}
}

void DES::IP(const int input[64], int output[64], int table[64]){
	int i;
	for (i = 0; i<64; i++){
		output[i] = input[table[i] - 1];
	}
}

void DES::E(const int input[32], int output[48], int table[48]){
	int i;
	for (i = 0; i<48; i++){
		output[i] = input[table[i] - 1];
	}
}

void DES::P(const int input[32], int output[32], int table[32]){
	int i;
	for (i = 0; i<32; i++){
		output[i] = input[table[i] - 1];
	}
}

void DES::IP_In(const int input[64], int output[64], int table[64]){
	int i;
	for (i = 0; i<64; i++){
		output[i] = input[table[i] - 1];
	}
}

void DES::PC_1(const int input[64], int output[56], int table[56]){
	int i;
	for (i = 0; i<56; i++){
		output[i] = input[table[i] - 1];
	}
}

void DES::PC_2(const int input[56], int output[48], int table[48]){
	int i;
	for (i = 0; i<48; i++){
		output[i] = input[table[i] - 1];
	}
}

void  DES::S(const int input[48], int output[32], int table[8][4][16]){
	int i = 0;
	int j = 0;
	int INT[8];
	for (; i<48; i = i + 6){
		INT[j] = table[j][(input[i] << 1) + (input[i + 5])][(input[i + 1] << 3) + (input[i + 2] << 2) + (input[i + 3] << 1) + (input[i + 4])];
		j++;
	}
	for (j = 0; j<8; j++){
		for (i = 0; i<4; i++){
			output[3 * (j + 1) - i + j] = (INT[j] >> i) & 1;
		}
	}
}

void DES::F_func(int input[32], int output[32], int subkey[48]){
	int len = 48;
	int temp[48] = { 0 };
	int temp_1[32] = { 0 };
	this->E(input, temp, E_Table);
	this->Xor(temp, subkey, len);
	this->S(temp, temp_1, S_Box);
	this->P(temp_1, output, P_Table);
}

void DES::RotateL(const int input[28], int output[28], int leftCount){
	int i;
	int len = 28;
	for (i = 0; i<len; i++){
		output[i] = input[(i + leftCount) % len];
	}
}

void DES::subKey_fun(const int input[64], int Subkey[16][48]){
	int loop = 1, loop_2 = 2;
	int i, j;
	int c[28], d[28];
	int pc_1[56] = { 0 };
	int pc_2[16][56] = { 0 };
	int rotatel_c[16][28] = { 0 };
	int rotatel_d[16][28] = { 0 };
	this->PC_1(input, pc_1, PC1_Table);
	for (i = 0; i<28; i++){
		c[i] = pc_1[i];
		d[i] = pc_1[i + 28];
	}
	int leftCount = 0;
	for (i = 1; i<17; i++){
		if (i == 1 || i == 2 || i == 9 || i == 16){
			leftCount += loop;
			this->RotateL(c, rotatel_c[i - 1], leftCount);
			this->RotateL(d, rotatel_d[i - 1], leftCount);
		}
		else{
			leftCount += loop_2;
			this->RotateL(c, rotatel_c[i - 1], leftCount);
			this->RotateL(d, rotatel_d[i - 1], leftCount);
		}
	}
	for (i = 0; i<16; i++){
		for (j = 0; j<28; j++){
			pc_2[i][j] = rotatel_c[i][j];
			pc_2[i][j + 28] = rotatel_d[i][j];
		}
	}
	for (i = 0; i<16; i++){
		this->PC_2(pc_2[i], Subkey[i], PC2_Table);
	}
}

void DES::DES_Efun(char input[8], char key_in[8], int output[64]){
	this->SetKey(key_in);
	int Ip[64] = { 0 };  
	int output_1[64] = { 0 };
	int subkeys[16][48];
	int chartobit[64] = { 0 };
	int key[64];
	int l[17][32], r[17][32];
	this->CharToBit(input, chartobit, 8); 
	this->IP(chartobit, Ip, IP_Table);  
	this->CharToBit(key_in, key, 8);  
	this->subKey_fun(key, subkeys);
	for (int i = 0; i<32; i++){
		l[0][i] = Ip[i];
		r[0][i] = Ip[32 + i];
	}
	for (int j = 1; j<16; j++){
		for (int k = 0; k<32; k++){
			l[j][k] = r[j - 1][k];
		}
		this->F_func(r[j - 1], r[j], subkeys[j - 1]);
		this->Xor(r[j], l[j - 1], 32);
	}
	int t = 0;
	for (t = 0; t<32; t++){
		r[16][t] = r[15][t];
	}
	this->F_func(r[15], l[16], subkeys[15]);
	this->Xor(l[16], l[15], 32);
	for (t = 0; t<32; t++){
		output_1[t] = l[16][t];
		output_1[32 + t] = r[16][t];
	}
	this->IP_In(output_1, output, IPR_Table);
}

void DES::DES_Dfun(int input[64], char output[8]){
	int Ip[64] = { 0 };
	int output_1[64] = { 0 };
	int output_2[64] = { 0 };
	int subkeys[16][48];
	int chartobit[64] = { 0 };
	int key[64];
	int l[17][32], r[17][32];
	this->IP(input, Ip, IP_Table);  
	this->CharToBit(this->GetKey(), key, 8);
	this->subKey_fun(key, subkeys);
	for (int i = 0; i<32; i++){
		l[0][i] = Ip[i];
		r[0][i] = Ip[32 + i];
	}
	for (int j = 1; j<16; j++){
		for (int k = 0; k<32; k++){
			l[j][k] = r[j - 1][k];
		}
		this->F_func(r[j - 1], r[j], subkeys[16 - j]);
		this->Xor(r[j], l[j - 1], 32);
	}
	int t = 0;
	for (t = 0; t<32; t++){
		r[16][t] = r[15][t];
	}
	this->F_func(r[15], l[16], subkeys[0]);
	this->Xor(l[16], l[15], 32);
	for (t = 0; t<32; t++){
		output_1[t] = l[16][t];
		output_1[32 + t] = r[16][t];
	}
	this->IP_In(output_1, output_2, IPR_Table);
	this->BitToChar(output_2, output, 8);
}

void DES::SetKey(char *key){
	strcpy(this->key, key);
}

char *DES::GetKey(){
	return this->key;
}

void DES::DisplayOutput(int *output){
	for (int i = 0; i<64; i++){
		printf("%d", output[i]);
		if ((i + 1) % 4 == 0)
			printf("\n");
	}
}

void DES::DisplayMessage(char *message){
	for (int i = 0; i<8; i++){
		printf("%c", message[i]);
	}
}