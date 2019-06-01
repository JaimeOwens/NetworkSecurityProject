#pragma once
#pragma warning(disable : 4996)
#include<iostream>
#include<cstring>
#define ch(x,y,z) (x)? y:z
#define maj(x,y,z) (x&y)^(y&z)^(x&z);
uint64_t rotr(int num, uint64_t t);
uint64_t detal(int flag, uint64_t t);
uint64_t sigma(int flag, uint64_t t);
class mblock
{
	char buff[128];
	uint64_t w[80];
public:
	mblock(char* buff);
	uint64_t initw(int num);
	uint64_t getw(int num);
	~mblock();
};


