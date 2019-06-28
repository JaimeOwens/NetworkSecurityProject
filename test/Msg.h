#pragma once
#include"rsa.h"
#include"SHA512.h"
#include"rc4.h"
#include<string>
class Msg
{
private:
	std::string pi;
	std::string oi;
	char ks[4096];
	int ds[16];
	RSA kr;
public:
	Msg();
	Msg(std::string ks,std::string pi, std::string oi,int n,int e,int d);
	unsigned char* makemsg(int a ,int b,int& lens);
	~Msg();
};

