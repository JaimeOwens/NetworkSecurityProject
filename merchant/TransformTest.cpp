#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

int main(){
	// char buffer1[128];
	// int *buffer2 = (int *)malloc(128*sizeof(int));
	// scanf("%s", buffer2);
	// printf("%s\n", buffer2);
	// //int to char
	// sprintf(buffer1, "%s" ,buffer2);
	// printf("%s\n", buffer1);
	// //char to int
	// int *buffer3;
	// buffer3 = (int *)&buffer1;
	// printf("%s\n", buffer3);
	char buffer1[128] = "123456789";
	char buffer2[128] = "234567891";
	char buffer3[128];
	memset(buffer3, 0, sizeof(buffer3));
	for(int i=0; i< strlen(buffer1); i++){
		buffer3[i] = buffer2[i] | buffer1[i]; 	
	}
	return 0;
}