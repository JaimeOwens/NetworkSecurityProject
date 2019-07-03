#include"Msg.h"
#include"Mytcp.h"
#define SHORTLENGTH 128
#define MIDLENGTH 256
#define LONGLENGTH 512
#define START 8
#define _RECVPORT_ 8886
#define _SENDPORT_ 8888
#define _BACKLOG_ 10
struct check_msg{
	char PIMD[MIDLENGTH];
	char OI[SHORTLENGTH];
	char DS[LONGLENGTH];
	int CC[2];
};
int main() {
	check_msg tmsg;
	std::string ks, oi, pi;
	int n, e, d;
	std::cin >> ks >> oi >> pi >> n >> e >> d;
	Msg m(ks,pi,oi,n,e,d);
	unsigned char* temp = NULL;
	int lens;
	temp = m.makemsg(1823347, 549,lens);
	std::cout<<"the whole len is "<<lens<<std::endl;
	int a=5, b=6,c=5;
	if (temp != NULL) {
		memcpy(&a, temp, 4);
		memcpy(&b, temp +4, 4);
		memcpy(&c,temp+8,4);
	}
	std::cout << a << " " << b <<" "<<c<<std::endl;
	unsigned char * msga = new unsigned char[a+1];
	int * msgb = new int[b/4];
	memcpy(msga,temp+12,a);
	memcpy(msgb,temp+12+a,b);
	memcpy(tmsg.PIMD,temp+12+a+b,128);
	memcpy(tmsg.OI,temp+12+a+b+128,128);
	memcpy(tmsg.DS,temp+12+a+b+256,c);
	memcpy(tmsg.CC,temp+12+a+b+768,8);
	std::cout<<"pimd is "<<tmsg.PIMD<<std::endl;
	uint64_t pimdnum[8];
	memcpy(pimdnum,tmsg.PIMD,128);
	for(int i =0;i<8;i++)
		std::cout<<pimdnum[i]<<std::endl;
	std::cout<<"the OI is    "<<tmsg.OI<<std::endl;
	std::cout<<tmsg.CC[0]<<" "<<tmsg.CC[1]<<std::endl;
	RSA ts;
	ts.SetPublicKey(1823347,549);
	ts.SetPrivateKey(1823347,990349);

	int * dss = new int[c/4];
	memcpy(dss,tmsg.DS,c);
	ts.decrypt(dss);
	std::string dsstr = ts.GetDecoded();
	unsigned short dsnum[64];
	mytcp se("127.0.0.1",8886);
	std::cout<<se.mysend(temp,lens);
	return 0;
}