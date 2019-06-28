#include "rc4.h"


void rc4::rc4_setup( unsigned char *s, unsigned char *key,  int length )
{
    int i, j;
    char k[256] = {0};
    unsigned char tmp = 0;

    i = 0;
    j = 0;

    for( i = 0; i < 256; i++ )
    {
        s[i] = i;
        k[i] = key[i % length];
    }

    for( i = 0; i < 256; i++ )
    {
        j = ( j + s[i] + k[i] )%256;
        tmp = s[i];
        s[i] = s[j]; 
        s[j] = tmp;
        //if( ++k >= length ) 
        //	k = 0;
    }
} 

void rc4::rc4_encrypt( unsigned char *s, unsigned char *data, int length )
{ 
    int i = 0, j = 0, t = 0, k;
    unsigned char tmp;

    cout<<"edata  : "<<data<<endl;
    for( k = 0; k < length; k++ ) 
    {
        i = (i + 1)%256;
        j = (j + s[i])%256;
        tmp = s[i];
        s[i] = s[j]; 
        s[j] = tmp;
        t = (s[i] + s[j])%256;

        data[k] ^= s[t];
     }
}

void rc4::rc4_decrypt( unsigned char *s, unsigned char *data, int length )
{ 
    int i = 0, j = 0, t = 0, k;
    unsigned char tmp;

    cout<<"edata  : "<<data<<endl;
    for( k = 0; k < length; k++ ) 
    {
        i = (i + 1)%256;
        j = (j + s[i])%256;
        tmp = s[i];
        s[i] = s[j]; 
        s[j] = tmp;
        t = (s[i] + s[j])%256;

        data[k] ^= s[t];
     }
}
/*
int main()
{
    char key[128] = {"12345678"};
    char data[512] = {"捕刺客打刺客"};
    unsigned char s[256] = {0};
    rc4 rr;

    cout<<"key : "<<key<<endl; 
    cout<<"raw : "<<data<<endl;
    rr.rc4_setup(s, (unsigned char *)key, strlen(key));  
    rr.rc4_encrypt(s, (unsigned char *)data, strlen(data));
    cout<<"encrypt  : "<<data<<endl;
    
    rr.rc4_setup(s, (unsigned char *)key, strlen(key));  
    rr.rc4_decrypt(s, (unsigned char *)data, strlen(data));
    cout<<"decrypt  : "<<data<<endl;
    return 0;
}
*/