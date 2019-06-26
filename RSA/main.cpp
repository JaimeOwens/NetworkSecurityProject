#include "rsa.h"

int main(int argc, char const *argv[]){
	RSA rsa;
	//1.创建密钥
	// rsa.CreateKeys();
	//2.导入密钥
	rsa.SetPrivateKey(4404199, 971);
	rsa.SetPublicKey(4404199, 2313307);
	//检查密钥
	int *privatekey, *publickey; 
	privatekey = rsa.GetPrivateKey();
	publickey = rsa.GetPublicKey();
	// cout<<*privatekey<<' '<<*(privatekey + 1)<<endl;
	// cout<<*publickey<<' '<<*(publickey + 1)<<endl;
	//加密解密过程
	char buffer[4096];
	int *encoded_temp;
	cin>>buffer;
	encoded_temp = rsa.encrypt(buffer);
	rsa.decrypt(encoded_temp);
	string encoded = rsa.GetEncoded();
	string decoded = rsa.GetDecoded();
	cout<<decoded<<endl;
	return 0;
}