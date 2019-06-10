#include "DES.h"

int main(int argc, char const *argv[]){
	DES des;
	int output[64] = {0};
	char message[9];
	char key[8];
	scanf("%s", message);
	scanf("%s", key);
	des.DES_Efun(message, key, output);
	//des.DisplayOutput(output);
	des.DES_Dfun(output, message);
	des.DisplayMessage(message);
	return 0;
}