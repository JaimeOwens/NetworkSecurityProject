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

const uint64_t SHA512::const_K[80] =
{
	0x428a2f98d728ae22, 0x7137449123ef65cd,
	0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
	0x3956c25bf348b538, 0x59f111f1b605d019,
	0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
	0xd807aa98a3030242, 0x12835b0145706fbe,
	0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
	0x72be5d74f27b896f, 0x80deb1fe3b1696b1,
	0x9bdc06a725c71235, 0xc19bf174cf692694,
	0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
	0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
	0x2de92c6f592b0275, 0x4a7484aa6ea6e483,
	0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
	0x983e5152ee66dfab, 0xa831c66d2db43210,
	0xb00327c898fb213f, 0xbf597fc7beef0ee4,
	0xc6e00bf33da88fc2, 0xd5a79147930aa725,
	0x06ca6351e003826f, 0x142929670a0e6e70,
	0x27b70a8546d22ffc, 0x2e1b21385c26c926,
	0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
	0x650a73548baf63de, 0x766a0abb3c77b2a8,
	0x81c2c92e47edaee6, 0x92722c851482353b,
	0xa2bfe8a14cf10364, 0xa81a664bbc423001,
	0xc24b8b70d0f89791, 0xc76c51a30654be30,
	0xd192e819d6ef5218, 0xd69906245565a910,
	0xf40e35855771202a, 0x106aa07032bbd1b8,
	0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
	0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
	0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
	0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
	0x748f82ee5defb2fc, 0x78a5636f43172f60,
	0x84c87814a1f0ab72, 0x8cc702081a6439ec,
	0x90befffa23631e28, 0xa4506cebde82bde9,
	0xbef9a3f7b2c67915, 0xc67178f2e372532b,
	0xca273eceea26619c, 0xd186b8c721c0c207,
	0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
	0x06f067aa72176fba, 0x0a637dc5a2c898a6,
	0x113f9804bef90dae, 0x1b710b35131c471b,
	0x28db77f523047d84, 0x32caab7b40c72493,
	0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
	0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
	0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};
SHA512::SHA512()
{
	mState[0] = 0x6a09e667f3bcc908;
	mState[1] = 0xbb67ae8584caa73b;
	mState[2] = 0x3c6ef372fe94f82b;
	mState[3] = 0xa54ff53a5f1d36f1;
	mState[4] = 0x510e527fade682d1;
	mState[5] = 0x9b05688c2b3e6c1f;
	mState[6] = 0x1f83d9abfb41bd6b;
	mState[7] = 0x5be0cd19137e2179;
}

void SHA512::init()
{
	mState[0] = 0x6a09e667f3bcc908;
	mState[1] = 0xbb67ae8584caa73b;
	mState[2] = 0x3c6ef372fe94f82b;
	mState[3] = 0xa54ff53a5f1d36f1;
	mState[4] = 0x510e527fade682d1;
	mState[5] = 0x9b05688c2b3e6c1f;
	mState[6] = 0x1f83d9abfb41bd6b;
	mState[7] = 0x5be0cd19137e2179;
}


hashval SHA512::hash(const char* words)
{
	this->init();
	uint64_t len = strlen(words);
	uint64_t  temp = len;
	int flag = len % 128;
	char* buf = new char[len +128-flag];
	strcpy(buf, words);
	buf[len] = 0x80;
	for (int i = len + 1; i < len + 128 - flag - 8; i++)
		buf[i] = 0x00;
	len = len + 128 - flag;
	memcpy(buf + len - 8, &temp, 8);
	int N = len / 128;
	char block[128];
	for (int i = 0; i < N; i++) {
		memcpy(block, buf + i * 128, 128);
		mblock T(block);
	//	for (int i = 0; i < 80; i++)
		//	std::cout << T.getw(i) << std::endl;
		//80 態
		uint64_t rtemp[8];
		for (int i = 0; i < 8; i++)
			rtemp[i] = this->mState[i];
		uint64_t t1, t2;
		uint64_t tempr;
		for (int i = 0; i < 80; i++)
		{
			t1 = this->mState[7] + ch(mState[4], mState[5], mState[6]) + sigma(1, mState[4]) + T.getw(i) + this->const_K[i];
			t2 = sigma(0, mState[0]) + maj(mState[0], mState[1], mState[2]);
			mState[7] = mState[6];
			mState[6] = mState[5];
			mState[5] = mState[4];
			mState[4] = mState[3] + t1;
			mState[3] = mState[2];
			mState[2] = mState[1];
			mState[1] = mState[0];
			mState[0] = t1 + t2;
		}
	}
	// 145*8= 17280 = 1024*1 + 136
	// 145 = 128*1 +17
	delete[]buf;
	hashval hval;
	for (int i = 0; i < 8; i++)
		hval.val[i] = this->mState[i];
//	memcpy(hval.val, this->mState, 512);
	return hval;
}

SHA512::~SHA512()
{
}
void printhash(hashval t) {
	std::string shows="";
	std::bitset<64> t1;
	for(int i=0;i<8;i++){
		t1 = t.val[i];
		shows+= t1.to_string();
	}
	std::cout<<shows<<std::endl;
}
