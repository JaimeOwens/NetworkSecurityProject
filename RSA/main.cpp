#include "rsa.h"

int main(int argc, char const *argv[]){
	//1.创建密钥
	// rsa.CreateKeys();
	//2.导入密钥
	// rsa.SetPrivateKey(1823347, 990349);
	// int *privatekey, *publickey; 
	// publickey = rsa.GetPublicKey();
	// privatekey = rsa.GetPrivateKey();
	//检查密钥
	// cout<<*privatekey<<' '<<*(privatekey + 1)<<endl;
	// cout<<*publickey<<' '<<*(publickey + 1)<<endl;
	//加密解密过程
	char buffer[4096];
	while(cin>>buffer){
		RSA rsa, rsa2;
		rsa.SetPublicKey(1823347, 549);
		rsa2.SetPrivateKey(1823347, 990349);
		int *encoded_temp = rsa.encrypt(buffer);
		int *p = encoded_temp;
		rsa2.decrypt(encoded_temp);
		string encoded = rsa.GetEncoded();
		string decoded = rsa2.GetDecoded();
		cout<<decoded<<endl;
		memset(buffer, 0, sizeof(buffer));
	}
	return 0;
}