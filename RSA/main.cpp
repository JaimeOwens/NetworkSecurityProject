#include "rsa.h"

int main(int argc, char const *argv[]){
	char buffer[4096];
	int *encoded_temp;
	RSA rsa;
	rsa.CreateKeys();
	cin>>buffer;
	// encoded_temp = rsa.encrypt(buffer);
	// rsa.decrypt(encoded_temp);
	int *privatekey, *publickey; 
	privatekey = rsa.GetPrivateKey();
	publickey = rsa.GetPublicKey();
	// string encoded = rsa.GetEncoded();
	// string decoded = rsa.GetDecoded();
	cout<<*privatekey<<' '<<*(privatekey + 1)<<endl;
	cout<<*publickey<<' '<<*(publickey + 1)<<endl;
	cout<<decoded<<endl;
	return 0;
}