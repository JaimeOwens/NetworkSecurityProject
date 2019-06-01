#pragma once
#include"SHA512.h"
int main() {
	SHA512 T;
	char t[] = "1asdfafwafhashfahuidhfiuahsiuvhaiufhiaufhaudhfaosufhoa";
	hashval hashbuff;
	hashbuff = T.hash(t);
	printhash(hashbuff);
	hashval h2 = T.hash(t);
	printhash(h2);
	return 0;
}