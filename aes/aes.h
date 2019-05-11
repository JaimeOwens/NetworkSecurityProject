#pragma once
#include"function.cpp"
class aes
{
private:
	w keyword[44];
	void roxword(gf28 word[4][4], w keyword[4]);
	void move(gf28 word[4][4]);
	void i_move(gf28 word[4][4]);
	void col_mix(gf28 word[4][4]);
	void i_col_mix(gf28 word[4][4]);
public:
	aes();
	aes(w keyword[44]);
	void encryption(unsigned char words[16]);
	void decrypt(unsigned char words[16]);
	~aes();
};

