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
	//加密 获得长度 拷贝加密的消息
	this->ds = this->kr.encrypt(pomdt);
	this->dslen = strlen(pomdt) / this->kr.GetBytes() * 4;
//	std::cout<<this->dslen<<std::endl;
	//memcpy(this->ds,this->kr.encrypt(pomdt),64);
	//now have ds pi oi 
}

unsigned char* Msg::makemsg(int kbn, int kbe,int&lens)
{
	int A_len = this->pi.size() + 64 + this->dslen;
	unsigned char* MSG_A = new unsigned char[this->pi.size() + 64 +this->dslen + 1];
	
	memset(MSG_A, 0, A_len );
	memcpy(MSG_A, this->pi.data(), this->pi.size());
	memcpy(MSG_A + this->pi.size(), this->ds, this->dslen);
	
	SHA512 t;
	hashval temphs = t.hash(this->oi.data());
	memcpy(MSG_A + this->pi.size() + this->dslen, &temphs.val, 64);
	
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
	lens = 12+len+A_len+520;

	unsigned char *msg = new unsigned char[lens+1];
	msg[lens] = '\0';
	memset(msg, 0, lens);
	
<<<<<<< HEAD
	memcpy(msg, &A_len, 4);
	memcpy(msg + 4, &len, 4);
	memcpy(msg + 8, MSG_A, A_len);
	cout<<"MSG_A: "<<MSG_A<<endl;
	memcpy(msg + 8 + A_len, kstemp, len);
	cout<<"MSG_B: "<<kstemp<<endl;
	temphs = t.hash(this->pi.data());
	memcpy(msg + 8 + A_len + len, &temphs.val, 64);
	cout<<"PIMD: "<<&temphs.val<<endl;
	memcpy(msg + 8 + A_len + len + 128, this->oi.data(), 64);
	cout<<"OI: "<<this->oi<<endl;
	memcpy(msg + 8 + A_len + len + 256, this->ds, 64);
	cout<<"DS: "<<this->ds<<endl;
	memcpy((msg + 8 + A_len + len + 384), this->kr.GetPublicKey(), 8);
=======
	memcpy(msg, &A_len, 4);//len a
	memcpy(msg + 4, &len, 4);//len b
	memcpy(msg + 8,&(this->dslen),4);//len ds
	//std::cout<<"the a_len and b_len is "<<A_len<<" "<<len<<std::endl;
	memcpy(msg + 12, MSG_A, A_len);
	memcpy(msg + 12 + A_len, kstemp, len);
	temphs = t.hash(this->pi.data());
	memcpy(msg + 12 + A_len + len, &temphs.val, 64);
	memcpy(msg + 12 + A_len + len + 128, this->oi.data(), 64);
	memcpy(msg + 12 + A_len + len + 256, this->ds, this->dslen);
	memcpy((msg + 12 + A_len + len + 512), this->kr.GetPublicKey(), 8);
>>>>>>> bd54a3879d41d2343f653dde4a69484c7f8d28a7
	return msg;
}
Msg::~Msg()
{
	delete []this->ds;
}
