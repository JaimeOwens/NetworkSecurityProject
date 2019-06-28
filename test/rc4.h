#ifndef __RC4_H
#define __RC4_H

#include<iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

class rc4
{
private:
	int x;
	int y;
	int m[256];
public:
	rc4(){};
	void rc4_setup( unsigned char *s, unsigned char *key,  int length );
	void rc4_encrypt( unsigned char *s, unsigned char *data, int length );
	void rc4_decrypt( unsigned char *s, unsigned char *data, int length );
	~rc4(){};
	
};
#endif