#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <ctime>
using namespace std;


#define ACCURACY 5
#define SINGLE_MAX 10000
#define EXPONENT_MAX 1000
#define BUF_SIZE 1024
#define BYTES 4

class RSA{
private:
	int n, e, d;
    int len, bytes, count;
    string encoded;
	string decoded;
public:
    RSA();
	void CreateKeys();
    int GetCount();
    int GetBytes();
    string GetEncoded();
    string GetDecoded();
    int *GetPublicKey();
    int *GetPrivateKey();
    void SetPublicKey(int n, int e);
    void SetPrivateKey(int n, int d);
	int gcd(int a, int b);
	int jacobi(int a, int n);
	int modpow(long long a, long long b, int c);
	int inverse(int n, int modulus);
	int randPrime(int n);
	int probablePrime(int n, int k);
	int solovayPrime(int a, int n);
	int randExponent(int phi, int n);
	int encode(int m, int e, int n);
	int decode(int c, int d, int n);
	int *encodeMessage(int len, int bytes, char* message, int exponent, int modulus);
	string decodeMessage(int len, int bytes, int* cryptogram, int exponent, int modulus);
    int *encrypt(char *buffer);
    void decrypt(int *encoded);
};
