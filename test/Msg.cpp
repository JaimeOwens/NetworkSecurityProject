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
	SHA512 t;
	hashval pimd = t.hash(this->pi.data());
	hashval oimd = t.hash(this->oi.data());
	hashval pomd;
	for (int i = 0; i < 8; i++) {
		pomd.val[i] = pimd.val[i] | oimd.val[i];
	}
	char pomdt[64];
	memcpy(pomdt, &pomd, 64);
	pomd = t.hash(pomdt);
	memset(pomdt, 0, 64);
	memcpy(pomdt, &pomd, 64);
	memcpy(this->ds,this->kr.encrypt(pomdt),64);
	//now have ds pi oi 
}

unsigned char* Msg::makemsg(int kbn, int kbe,int&lens)
{
	int A_len = this->pi.size() + 128+1;
	unsigned char* MSG_A = new unsigned char[this->pi.size() + 128 + 1];
	
	memset(MSG_A, 0, A_len );
	memcpy(MSG_A, this->pi.data(), this->pi.size());
	memcpy(MSG_A + this->pi.size(), this->ds, 64);
	
	SHA512 t;
	hashval temphs = t.hash(this->oi.data());
	memcpy(MSG_A + this->pi.size() + 64, &temphs.val, 64);
	
	rc4 rc;
	MSG_A[A_len] = '\0';
	char s[256] = { 0 };
	rc.rc4_setup((unsigned char *)s, (unsigned char*)this->ks, strlen(this->ks));
	rc.rc4_encrypt((unsigned char*)s, MSG_A, A_len);
	
	RSA kb;
	kb.SetPublicKey(kbn, kbe);
	int* kstemp = kb.encrypt(this->ks);
	kb.decrypt(kstemp);
	
	int len = strlen(this->ks) / kb.GetBytes() * 4;
	lens = 8+len+A_len+386;

	unsigned char *msg = new unsigned char[8 + len + A_len + 386+1];
	msg[lens] = '\0';
	memset(msg, 0, 8 + len + A_len + 386);
	
	memcpy(msg, &A_len, 4);
	memcpy(msg + 4, &len, 4);
	memcpy(msg + 8, MSG_A, A_len);
	memcpy(msg + 8 + A_len, kstemp, len);
	temphs = t.hash(this->pi.data());
	memcpy(msg + 8 + A_len + len, &temphs.val, 64);
	memcpy(msg + 8 + A_len + len + 128, this->oi.data(), 64);
	memcpy(msg + 8 + A_len + len + 256, this->ds, 64);
	memcpy((msg + 8 + A_len + len + 384), this->kr.GetPublicKey(), 8);
	return msg;
}
Msg::~Msg()
{
}
