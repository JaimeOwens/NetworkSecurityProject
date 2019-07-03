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
	temp = m.makemsg(n, d, lens);
	std::cout<<"the whole len is "<<lens<<std::endl;
	int a=5, b=6,c=5;
	if (temp != NULL) {
		memcpy(&a, temp, 4);
		memcpy(&b, temp +4, 4);
		memcpy(&c,temp+8,4);
	}
	mytcp se("127.0.0.1",8886);
	std::cout<<se.mysend(temp,lens);
	delete []temp;
	return 0;
}
