#include"lib.h"
#include"aes.cpp"
int main() {
	w keyword[44];
  //  inputkey("1234568798456456",keyword);
  
    keyword[0].elem[0] = (unsigned char)0x0f;
    keyword[0].elem[1] = (unsigned char)0x15;
    keyword[0].elem[2] = (unsigned char)0x71;
    keyword[0].elem[3] = (unsigned char)0xc9;
    keyword[1].elem[0] = (unsigned char)0x47;
    keyword[1].elem[1] = (unsigned char)0xd9;
    keyword[1].elem[2] = (unsigned char)0xe8;
    keyword[1].elem[3] = (unsigned char)0x59;
    keyword[2].elem[0] = (unsigned char)0x0c;
    keyword[2].elem[1] = (unsigned char)0xb7;
    keyword[2].elem[2] = (unsigned char)0xad;
    keyword[2].elem[3] = (unsigned char)0xd6;
    keyword[3].elem[0] = (unsigned char)0xaf;
    keyword[3].elem[1] = (unsigned char)0x7f;
    keyword[3].elem[2] = (unsigned char)0x67;
    keyword[3].elem[3] = (unsigned char)0x98;
    
    makekeyword(keyword);
    aes t(keyword);
    unsigned char b[17];
    b[16] = '\0';
    std::cin>>b;
    t.encryption(b);
    std::cout<<b<<std::endl;
   t.decrypt(b);
    std::cout<<b<<std::endl;
	return 0;
}
