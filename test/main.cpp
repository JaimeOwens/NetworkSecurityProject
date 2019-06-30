#include"Msg.h"
#include"Mytcp.h"
int main() {
	std::string ks, oi, pi;
	int n, e, d;
	std::cin >> ks >> oi >> pi >> n >> e >> d;
	Msg m(ks,pi,oi,n,e,d);
	unsigned char* temp = NULL;
	int lens;
	temp = m.makemsg(1823347, 549,lens);
	int a=5, b=6,c=5;
	if (temp != NULL) {
		memcpy(&a, temp+lens-8, 4);
		memcpy(&b, temp +lens-4, 4);
	}
	std::cout << a << " " << b <<" "<<c<<std::endl;
	std::cout<<(char*)(temp+12)<<std::endl;
	int * te = new int[c];
	//memcpy(temp+12+)
	mytcp se("127.0.0.1",8886);
	std::cout<<se.mysend(temp,lens);
	//int * key = new int[b/4];
//	memcpy(temp+8+a,key,b);
	return 0;
}