#include "mblock.h"

mblock::mblock(char * buff)
{
	strcpy(this->buff, buff);
	for (int i = 0; i < 80; i++)
		this->initw(i);
}

uint64_t mblock::initw(int num)
{
	uint64_t temp;
	if (num < 16)
		memcpy(&temp, this->buff + num * 8, 8);
	else
		temp = detal(1,initw(num - 2)) + initw(num - 7) + detal(0, initw(num - 15)) + initw(num - 16);
	return uint64_t();
}

uint64_t mblock::getw(int num)
{
	return this->w[num];
}


mblock::~mblock()
{
}
uint64_t rotr(int num, uint64_t t) {
	return (t >> num) | (t << 64 - num);
}
uint64_t detal(int flag, uint64_t t)
{
	uint64_t temp = t;
	uint64_t s;
	if (flag == 0) {
		//(x >> n) | (x << (s - n));
		s = rotr(1,temp);
		s = s + rotr(8, temp);
		s = s >> 7;
	}
	else {
		s = rotr(19, temp);
		s = s + rotr(61, temp);
		s = s >> 6;
	}
	return s;
}
uint64_t sigma(int flag, uint64_t t) {
	uint64_t s = t;
	if (flag == 0) {
		s = rotr(28, t);
		s = s ^ rotr(34, t);
		s = s * rotr(39, t);
	}
	else {
		s = rotr(14, t) ^ rotr(18, t) ^ rotr(41, t);
	}
	return s;
}