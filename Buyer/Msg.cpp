#include "Msg.h"
Msg::Msg()
{

}

Msg::Msg(std::string ks,std::string pi, std::string oi,int n,int e,int d)
{
	this->pi = pi;
	this->oi = oi;
	this->kr.SetPrivateKey(n, e);
	this->kr.SetPublicKey(n, d);
	//this->ks = new char[ks.size() + 1];
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

unsigned char* Msg::makemsg(int kbn, int kbe)
{
	//加密的 PI +DS + OIMD
	//need ks use rc4
	
	int A_len = this->pi.size() + 128+1;
	unsigned char* MSG_A = new unsigned char[this->pi.size() + 128 + 1];
	memset(MSG_A, 0, A_len );
	memcpy(MSG_A, this->pi.data(), this->pi.size());
	memcpy(MSG_A + this->pi.size(), this->ds, 64);
	
	SHA512 t;
	hashval temphs = t.hash(this->oi.data());
	memcpy(MSG_A + this->pi.size() + 64, &temphs.val, 64);
	
	//MSG_A GET
	
	rc4 rc;
	MSG_A[A_len] = '\0';
	char s[256] = { 0 };
	std::cout << MSG_A << std::endl;
	rc.rc4_setup((unsigned char *)s, (unsigned char*)this->ks, strlen(this->ks));
	rc.rc4_encrypt((unsigned char*)s, MSG_A, A_len);
	std::cout << MSG_A << std::endl;
	//rc.rc4_setup((unsigned char*)s, (unsigned char*)this->ks, strlen(this->ks));
	//rc.rc4_decrypt((unsigned char*)s, MSG_A, A_len);
	std::cout << MSG_A << std::endl;
	//rc.rc4_encrypt()
	//have en MSG_A

	RSA kb;
	kb.SetPublicKey(kbn, kbe);
	kb.SetPrivateKey(4404199, 971);
	//std::cout << "now is bank kub\n";
	std::cout << this->ks << "    ks desu"<<std::endl;
	int* kstemp = kb.encrypt(this->ks);
	
	kb.decrypt(kstemp);
	std::cout << kb.GetDecoded() << std::endl;

	//std::cout << this->ks << std::endl;
	kb.decrypt(kstemp);
	//ks  加密有了
	// A_len len
	int len = strlen(this->ks) / kb.GetBytes() * 4;
	std::cout << len << " len " << A_len<<"     a_len"<<std::endl;

	unsigned char *msg = new unsigned char[8 + len + A_len + 386+1];
	
	memset(msg, 0, 8 + len + A_len + 386);
	memcpy(msg, &A_len, 4);
	memcpy(msg + 4, &len, 4);

memcpy(msg + 8, MSG_A, A_len);
	
memcpy(msg + 8 + A_len, kstemp, len);
	//now use pimd oi ds cc
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
