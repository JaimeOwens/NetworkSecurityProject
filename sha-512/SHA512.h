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
	uint64_t mState[8];//缓存器
	static const uint64_t const_K[80];
public:
	SHA512();
	void init();
	//hashval hash(char*);
	hashval hash(const char*);
	~SHA512();
	/*
	todo 
	1 切片函数 将组分出来 或者用vector存1024
	2 消息导出 每一轮用临时变量
	3 轮函数
	 4 可能有加法函数 或者定义一个类来操作每个块
	*/
};


