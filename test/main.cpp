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
	int a=5, b=6;
	if (temp != NULL) {
		memcpy(&a, temp, 4);
		memcpy(&b, temp + 4, 4);
	}
	std::cout << a << " " << b << std::endl;
	mytcp se("127.0.0.1",8886);
	se.mysend(temp,lens);
	//int * key = new int[b/4];
//	memcpy(temp+8+a,key,b);
	return 0;
}