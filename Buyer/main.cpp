#include"Msg.h"
#include"Mytcp.h"
int main() {
	std::string ks, oi, pi;
	int n, e, d;
	std::cin >> ks >> oi >> pi >> n >> e >> d;
	Msg m(ks, pi, oi, n, e, d);
	unsigned char* temp = NULL;
	int lens;
	temp = m.makemsg(1823347, 549, lens);
	mytcp se("127.0.0.1",8886);
	se.mysend(temp, lens);
	char buf[4096];
	se.myrecv(buf);
	return 0;
}