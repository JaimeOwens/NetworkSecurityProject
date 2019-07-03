#include<iostream>
#include<string>
#include"SHA512.h"
#include"rsa.h"
int main(){
     std::string pi,oi,pimd,oimd;
    std::cin>>pi>>oi;
    pimd = sha512(pi);
    oimd = sha512(oi);
    char temp[64];
    for(int i = 0;i<64;i++)
        temp[i] = pimd[i]|oimd[i];
    std::string pomd = sha512(temp,64);
    uint64_t val[8];
    block(pomd,val);
    
    unsigned short tds[32];
    memcpy(tds,val,64);
   for(int i =0;i<32;i++)
        std::cout<<tds[i]<<" ";
    std::cout<<"\n";
    RSA rsa;
    rsa.CreateKeys();
    int *en = rsa.GetPublicKey();
    int *dn = rsa.GetPrivateKey();
    int *ttds = rsa.encrypt((char*)tds);
    std::cout<<rsa.GetCount()<<std::endl;
    int *ttss = new int[rsa.GetCount()*sizeof(int)];
    memcpy(ttss,ttds,rsa.GetCount()*sizeof(int));
    rsa.decrypt(ttss);
    std::string tdss = rsa.GetDecoded();
    memcpy(tds,tdss.data(),64);
    for(int i =0;i<32;i++)
        std::cout<<tds[i]<<" ";
    std::cout<<"\n";
    return 0;
}