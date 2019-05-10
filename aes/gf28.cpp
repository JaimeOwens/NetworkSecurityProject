#include"gf28.h"
gf28::gf28(char temp){
    for(int i = 0;i<8;i++){
        bool t= temp&0x01;
        this->elem.set(i,t);
        temp =temp>>1;
    }
    for(int i = 8;i<16;i++)
        this->elem.set(i,0);
}
gf28::gf28(std::bitset<16>a){
    this->elem = a;
}
gf28 gf28::operator *(const gf28 & b){
    std::bitset<16> base(std::string("0000000100011011"));
    std::bitset<16> temp;
    temp.reset();
    for(int i = 0;i<16;i++){
        if(b.elem[i]==1){
            
            temp^=(this->elem<<i);
        }
    }
    for(int i = 15;i>=8;i--){
        if(temp[i]==1){
            temp = temp^(base<<(i-8));
        }
    }
    return gf28(temp);
}
