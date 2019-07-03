#include"SHA512.h"
int main(){
    std::string pi;
    std::string oi;
    std::cin>>pi>>oi;
    std::string pimd = sha512(pi);
    std::string oimd = sha512(oi);
    char pomd[64];
    for(int i =0;i<64;i++)
        pomd[i] = pimd[i]|oimd[i];
    std::string la = sha512(pomd,64);
    uint64_t val[8];
    block(la,val);
    for(int i =0;i<8;i++)
        std::cout<<val[i]<<std::endl;
}