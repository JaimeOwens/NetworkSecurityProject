#pragma once
#include"mblock.h"

#include<bitset>
struct hashval {
	uint64_t val[8];
};
void printhash(hashval t);
class SHA512
{
private:
	uint64_t mState[8];//������
	static const uint64_t const_K[80];
public:
	SHA512();
	void init();
	//hashval hash(char*);
	hashval hash(const char*);
	~SHA512();
	/*
	todo 
	1 ��Ƭ���� ����ֳ��� ������vector��1024
	2 ��Ϣ���� ÿһ������ʱ����
	3 �ֺ���
	 4 �����мӷ����� ���߶���һ����������ÿ����
	*/
};


