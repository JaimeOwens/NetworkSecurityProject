#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

int main(){
	int *intp = new int[10];
	char temp[10*4+1] = "0123456789012345678901234567890123456789";
	for(int i=0, *p = (int *)temp; i<10; i++){
		*intp++ = *p++;
		cout<<*intp<<endl;
	}

	return 0;
}