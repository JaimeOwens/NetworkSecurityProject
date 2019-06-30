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
    int GetLength(int *);
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

RSA::RSA(){
    this->len = 0;
}

int RSA::modpow(long long a, long long b, int c) {
    int res = 1;
    while(b > 0) {
        if(b & 1) {
            res = (res * a) % c;
        }
        b = b >> 1;
        a = (a * a) % c; 
    }
    return res;
}

int RSA::jacobi(int a, int n){
	int twos, temp;
    int mult = 1;
    while(a > 1 && a != n) {
        a = a % n;
        if(a <= 1 || a == n) break;
        twos = 0;
        while(a % 2 == 0 && ++twos) a /= 2; 
        if(twos > 0 && twos % 2 == 1) mult *= (n % 8 == 1 || n % 8 == 7) * 2 - 1;
        if(a <= 1 || a == n) break;
        if(n % 4 != 1 && a % 4 != 1) mult *= -1; 
        temp = a;
        a = n;
        n = temp;
    }
    if(a == 0) return 0;
    else if(a == 1) return mult;
    else return 0; 
}

int RSA::solovayPrime(int a,int n){
	int x = jacobi(a, n);
    if(x == -1) x = n - 1;
    return x != 0 && modpow(a, (n - 1)/2, n) == x;
}

int RSA::probablePrime(int n, int k){
	if(n == 2) return 1;
	else if(n % 2 == 0 || n == 1) return 0;
    while(k-- > 0) {
        if(!solovayPrime(rand() % (n - 2) + 2, n)) return 0;
    }
    return 1;
}

int RSA::randPrime(int n){
	int prime = rand() % n;
    n += n % 2; 
    prime += 1 - prime % 2;
    while(1) {
        if(probablePrime(prime, ACCURACY)) return prime;
        prime = (prime + 2) % n;
    }
}

int RSA::gcd(int a, int b) {
    int temp;
    while(b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int RSA::randExponent(int phi, int n) {
    int e = rand() % n;
    while(1) {
        if(gcd(e, phi) == 1) return e;
        e = (e + 1) % n;
        if(e <= 2) e = 3;
    }
}

int RSA::inverse(int n, int modulus){
	int a = n, b = modulus;
    int x = 0, y = 1, x0 = 1, y0 = 0, q, temp;
    while(b != 0) {
        q = a / b;
        temp = a % b;
        a = b;
        b = temp;
        temp = x; x = x0 - q * x; x0 = temp;
        temp = y; y = y0 - q * y; y0 = temp;
    }
    if(x0 < 0) x0 += modulus;
    return x0;
}

void RSA::CreateKeys(){
    int p, q, phi;
    srand(time(NULL));
    while(1){
        p = this->randPrime(SINGLE_MAX);
        // printf("Got first prime factor, p = %d ... \n", p);
 
        q = this->randPrime(SINGLE_MAX);
        // printf("Got second prime factor, q = %d ... \n", q);
 
        this->n = p * q;
        // printf("Got modulus, n = pq = %d ... \n", this->n);
        if(this->n < 128) {
            printf("Modulus is less than 128, cannot encode single bytes. Trying again ... \n");
        }
        else break;
    }
    if(this->n >> 21) this->bytes = 3;
    else if(this->n >> 14) this->bytes = 2;
    else this->bytes = 1;

    phi = (p - 1) *(q - 1);
    // printf("Got totient, phi = %d ... \n", phi);

    this->e = this->randExponent(phi, EXPONENT_MAX);
    // printf("Chose public exponent, e = %d\nPublic key is (%d, %d) ... \n", this->e, this->e, n);
 
    this->d = this->inverse(this->e, phi);
    // printf("Calculated private exponent, d = %d\nPrivate key is (%d, %d) ... \n", this->d, this->d, n);

    return;    
}

int RSA::encode(int m, int e, int n){
	return modpow(m, e, n);
}

int *RSA::encodeMessage(int len, int bytes, char* message, int exponent, int modulus) {
    int *encoded = (int *)malloc((len/bytes) * sizeof(int));
    int x, i, j;
    int count = 0;
    for(i = 0; i < len; i += bytes) {
        x = 0;
        for(j = 0; j < bytes; j++) 
            x += message[i + j] * (1 << (7 * j));
        encoded[i/bytes] = encode(x, exponent, modulus);
        count ++;
        // printf("%d ", encoded[i/bytes]);
    }
    // cout<<endl;
    int *p = encoded;
    int *encoded2 = (int *)malloc(sizeof(int) * count);
    this->count = count;
    int *q = encoded2;
    while(count--){
        // cout<<*p<<' ';
        *q = *p;
        p ++, q ++;
    }
    if(strlen(message)%BYTES == 3 ||strlen(message)%BYTES == 0)
        *q = NULL;
    // cout<<endl; 
    // cout<<"bytes1: "<<this->bytes<<endl;
    // cout<<"count1: "<<this->count<<endl;
    // cout<<"len1: "<<this->len<<endl;
    return encoded2;
}

int *RSA::encrypt(char *buffer){
    // cout<<"strlen: "<<strlen(buffer)<<endl;
    this->len = strlen(buffer);
    if(this->len%BYTES != 0){
        this->len = (this->len - this->len % BYTES) + BYTES;
    }
    int *encoded;
    encoded = this->encodeMessage(this->len, this->bytes, buffer, this->e, this->n);
    return encoded;
}

int RSA::decode(int c, int d, int n) {
    return modpow(c, d, n);
}

string RSA::decodeMessage(int len, int bytes, int* cryptogram, int exponent, int modulus) {
    int *decoded = (int *)malloc(len * bytes * sizeof(int));
    int x, i, j;
    for(i = 0; i < len; i++) {
        x = decode(cryptogram[i], exponent, modulus);
        for(j = 0; j < bytes; j++) {
            decoded[i*bytes + j] = (x >> (7 * j)) % 128;
            if(decoded[i*bytes + j] != '\0') 
                this->decoded += decoded[i*bytes + j];
                // printf("%c ", decoded[i*bytes + j]);
        }
    }
    // printf("\n");
    return this->decoded;
}

void RSA::decrypt(int *encoded){
    int count = 0;
    int flag = 0;
    int *p = encoded;
    while(*p != 0){
        // cout<<*p<<' ';
        p ++;
        count ++;
    }
    // cout<<endl;
    this->len = count * this->bytes;
    this->count = count;
    this->decoded = this->decodeMessage(this->len/this->bytes, this->bytes, encoded, this->d, this->n);
    // cout<<"bytes2: "<<this->bytes<<endl;
    // cout<<"count2: "<<this->count<<endl;
    // cout<<"len2: "<<this->len<<endl;
}

string RSA::GetEncoded(){
    return this->encoded;
}

string RSA::GetDecoded(){
    return this->decoded;
}

int *RSA::GetPrivateKey(){
    static int privatekey[2];
    privatekey[0] = this->n;
    privatekey[1] = this->d;
    printf("privatekey: n->%d d->%d\n", this->n, this->d);
    return privatekey;
}


int *RSA::GetPublicKey(){
    static int publickey[2];
    publickey[0] = this->n;
    publickey[1] = this->e;
    printf("publickey: n->%d e->%d\n", this->n, this->e);
    return publickey;
}

void RSA::SetPrivateKey(int n, int d){
    if(n >> 21) this->bytes = 3;
    else if(n >> 14) this->bytes = 2;
    else this->bytes = 1;
    this->n = n;
    this->d = d;
}

void RSA::SetPublicKey(int n, int e){
    if(n >> 21) this->bytes = 3;
    else if(n >> 14) this->bytes = 2;
    else this->bytes = 1;
    this->n = n;
    this->e = e;
}

int RSA::GetBytes(){
    return this->bytes;
}

int RSA::GetCount(){
    return this->count;
}

int RSA::GetLength(int *encoded){
    int count = 0;
    int *p = encoded;
    while(*p != 0){
        p ++;
        count ++;
    } 
    int len = count * sizeof(int);
    return len;
}

