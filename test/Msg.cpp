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
	char temp[64];
	for (int i = 0; i < 64; i++) {
		temp[i] = pimd[i]|oimd[i];
	}
	std::string pomd = sha512(temp,64);
	  unsigned short tds[32];
    memcpy(tds,pomd.data(),64);
    int * tempds = this->kr.encrypt((char*)tds);
	this->dslen = kr.GetCount()*sizeof(int);
	this->ds = new int[this->dslen];
	//std::cout<<"dslen "<<this->dslen<<std::endl;
	memcpy(this->ds,tempds,dslen);

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
	std::cout<<"msg_a has ready\n";
	rc4 rc;
	MSG_A[A_len] = '\0';
	char s[256] = { 0 };
	rc.rc4_setup((unsigned char *)s, (unsigned char*)this->ks, strlen(this->ks));
	rc.rc4_encrypt((unsigned char*)s, MSG_A, A_len);
	std::cout<<"rc4 encrypt is ready\n";
	RSA kb;
	kb.SetPublicKey(kbn, kbe);
	int* kstemp = kb.encrypt(this->ks);
	kb.decrypt(kstemp);
	std::cout<<"de is ready\n";
	int len = kb.GetCount()*sizeof(int);
	lens = 12+len+A_len+520;

	unsigned char *msg = new unsigned char[lens+1];
	msg[lens] = '\0';
	memset(msg, 0, lens);
	
	memcpy(msg, &A_len, 4);//len a
	memcpy(msg + 4, &len, 4);//len b
	memcpy(msg + 8, &(this->dslen), 4);//len ds
	cout<<A_len<<' '<<len<<' '<<this->dslen<<endl;
	//std::cout<<"the a_len and b_len is "<<A_len<<" "<<len<<std::endl;
	memcpy(msg + 12, MSG_A, A_len);
	memcpy(msg + 12 + A_len, kstemp, len);
	temphs = sha512(this->pi.data());
	memcpy(msg + 12 + A_len + len, temphs.data(), 64);
	memcpy(msg + 12 + A_len + len + 128, this->oi.data(), 64);
	memcpy(msg + 12 + A_len + len + 256, this->ds, this->dslen);
	memcpy((msg + 12 + A_len + len + 512), this->kr.GetPublicKey(), 8);
	cout<<endl;
	cout<<"MSG_A: "<<MSG_A<<endl;
	cout<<"MSG_B: "<<kstemp<<endl;
	cout<<"PIMD: "<<temphs.data()<<endl;
	cout<<"OI: "<<this->oi.data()<<endl;
	cout<<"DS: "<<this->ds<<endl;
	return msg;
}
Msg::~Msg()
{
	delete []this->ds;
}
