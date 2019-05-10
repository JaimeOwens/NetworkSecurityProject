#include"lib.h"
#include"gf28.cpp"
#include"bitmartix.cpp"
struct w{
    gf28 elem[4];
};
gf28 s(gf28 a){
    bitmartix s(8,8);
 std::string ss = "10001111";
            for(int i =0;i<8;i++){
                s.setrow(ss,i);
                std::string bit = ss.substr(7,1);
                ss = ss.substr(0,ss.size()-1);
                ss = bit+ss;
            }
    a = !a;
    bitmartix temp(8,1);
    temp.setcol(a.getstrval(),0);
    bitmartix base(8,1);
    base.setcol(std::string("01100011"),0);
    ss = (s*temp+base).to_bits();
    for(int i = 0;i<4;i++){
        char bi = ss[i];
        ss[i] = ss[7-i];
        ss[7-i] = bi; 
    }
    return gf28(std::bitset<16>(ss));
}
gf28 s_1(gf28 a){
    bitmartix s_1(8,8);
    s_1.setrow("00100101",0);
    s_1.setrow("10010010",1);
    s_1.setrow("01001001",2);
    s_1.setrow("10100100",3);
    s_1.setrow("01010010",4);
    s_1.setrow("00101001",5);
    s_1.setrow("10010100",6);
    s_1.setrow("01001010",7);
    std::string ss;
    bitmartix temp(8,1);
    temp.setcol(a.getstrval(),0);
    bitmartix base(8,1);
    base.setcol(std::string("01100011"),0);
    ss = (s_1*(temp+base)).to_bits();
    for(int i = 0;i<4;i++){
        char bi = ss[i];
        ss[i] = ss[7-i];
        ss[7-i] = bi; 
    }
    return !gf28(std::bitset<16>(ss));
}
w g(w am,int j){
    w a = am;
    gf28 temp = a.elem[0];
    for(int i = 0;i<3;i++)
        a.elem[i] = a.elem[i+1];
    a.elem[3] = temp;
    for(int i = 0;i<4;i++){
        a.elem[i] = s(a.elem[i]);
    }
    gf28 rc(0x01);
    gf28 times(0x02);
    for(int i = 1;i<j;i++)
        rc = rc*times;
    a.elem[0] = a.elem[0]+rc;
    return a;
}
void makekeyword(w keyword[44]){
    for(int i = 4;i<44;i++){
        w temp = keyword[i-1];
        if(i%4==0)
            temp = g(keyword[i-1],i/4);
            for(int wi = 0;wi<4;wi++){
                keyword[i].elem[wi] = keyword[i-4].elem[wi]+temp.elem[wi];
            }
    }
}