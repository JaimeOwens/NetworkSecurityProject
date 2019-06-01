#pragma once
#include"SHA512.h"
int main() {
	SHA512 T;
	char t[] = "1asdfafwafhashfahuidhfiuahsiuvhaiufhiaufhaudhfaosufhoa";
	hashval hashbuff;
	hashbuff = T.hash(t);
	printhash(hashbuff);
	char t2[] = "1asdfafwafhashfahuidhfiuahsiuvhaiufhiaufhaudhfaosufho4";
	hashval h2 = T.hash(t2);
	printhash(h2);
	return 0;
}