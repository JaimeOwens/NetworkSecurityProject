#include "Msg.h"
Msg::Msg()
{

}
Msg::Msg(std::string ks,std::string pi, std::string oi,int n,int e,int d)
{
	this->pi = pi;
	this->oi = oi;
	this->kr.SetPrivateKey(n, d);
	this->kr.SetPublicKey(n, e);
	strcpy(this->ks, ks.data());
	std::string pimd = sha512(pi);
    std::string oimd = sha512(oi);
	char temp[128];
	for (int i = 0; i < 128; i++) {
		temp[i] = pimd[i]|oimd[i];
	}
	std::string pomd = sha512(temp,128);
	  unsigned short tds[64];
    memcpy(tds,pomd.data(),128);
	this->ds = this->kr.encrypt((char*)tds);
	this->dslen = this->kr.GetLength(this->ds);
	//this->ds = new int[this->dslen/4];
	//memcpy(this->ds,tempds,dslen);
}

unsigned char* Msg::makemsg(int kbn, int kbe,int&lens)
{
	int A_len = this->pi.size() + 64 + this->dslen;
	unsigned char* MSG_A = new unsigned char[this->pi.size() + 64 +this->dslen + 1];
	
	memset(MSG_A, 0, A_len);
	memcpy(MSG_A, this->pi.data(), this->pi.size());
	memcpy(MSG_A + this->pi.size(), this->ds, this->dslen);
	
	std::string temphs = sha512(this->oi.data());
	memcpy(MSG_A + this->pi.size() + this->dslen, temphs.data(), 64);
	rc4 rc;
	MSG_A[A_len] = '\0';
	char s[256] = { 0 };
	rc.rc4_setup((unsigned char *)s, (unsigned char*)this->ks, strlen(this->ks));
	rc.rc4_encrypt((unsigned char*)s, MSG_A, A_len);
	RSA kb;
	kb.SetPublicKey(kbn, kbe);
	int* kstemp = kb.encrypt(this->ks);
	
	int len = kb.GetLength(kstemp);
	lens = 12+len+A_len+768+8;
	unsigned char *msg = new unsigned char[lens+1];
	msg[lens] = '\0';
	memset(msg, 0, lens);
	memcpy(msg, &A_len, 4);//len a
	memcpy(msg + 4, &len, 4);//len b
	memcpy(msg + 8,&(this->dslen),4);//len ds
	memcpy(msg + 12, MSG_A, A_len);//msg_a 
	memcpy(msg + 12 + A_len, kstemp, len);//ks
	std::string ttemphs = sha512(this->pi.data());
	memcpy(msg + 12 + A_len + len, ttemphs.c_str(), 128);//pimd
	memcpy(msg + 12 + A_len + len + 128, this->oi.data(), this->oi.size());//oi 
	msg[12+A_len+len+128+this->oi.size()]='\0';
	memcpy(msg + 12 + A_len + len + 256, this->ds, this->dslen);//DS 512

	memcpy((msg + 12 + A_len + len +768 ), this->kr.GetPublicKey(), 8);
	return msg;
}
Msg::~Msg()
{
//	delete []this->ds;
}
