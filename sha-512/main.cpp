#pragma once
#include"SHA512.h"
int main() {
	char t[] = "1asdfafwafhashfahuidhfiuahsiuvhaiufhiaufhaudhfaosufhoa";
	std::string hash = hash512(t);
	hash = hash512(t,strlen(t));
	return 0;
}