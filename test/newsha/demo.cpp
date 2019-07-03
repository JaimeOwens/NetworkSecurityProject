#include"rsa.h"
#include"SHA512.h"
int quickMod(int a,int b,int n){
   // std::cout<<" a "<<a<<std::endl;
    int ans = 1;
	while (b)
	{  
		ans = (ans*a)%n;
        b--;
	}
	return ans;
}
hashval encrypt(hashval t,int n,int e){
 //   hashval tempa;
    for(int i = 0;i<8;i++){
        uint64_t temp = t.val[i];
        unsigned char t1[8];
        memcpy(t1,&temp,8);
        for(int i = 0;i<4;i++){
            std::cout<<(unsigned int)t1[i]<<" ";
            t1[i] = quickMod((int)t1[i],e,n);
        }
        std::cout<<"\n";
        for(int i =0;i<4;i++)
            std::cout<<(unsigned int)t1[i]<<" ";
        std::cout<<"\n\n";
        memcpy(&temp,t1,8);
        t.val[i] = temp;
    }
    return t;
}
int main(){
    std::string a;
    std::string b;
    std::cin>>a>>b;
    hashval pi,oi;
    SHA512 t;
    pi = t.hash(a.data());
    oi = t.hash(a.data());
    hashval pomd;
    for(int i =0;i<8;i++)
        std::cout<<pi.val[i]<<" "<<oi.val[i]<<std::endl;
   // char pomdt[64];
	//memcpy(pomdt, pomd.val, 64);
//	pomd = t.hash(pomdt);
// for(int i=0 ;i<8;i++)
  //      std::cout<<pomd.val[i]<<std::endl;
   // std::cout<<"\n";
   /*  hashval te2 = encrypt(pomd,40921,211);
    te2 = encrypt(te2,40921,16891);

   // char tes[64];
   // memcpy(tes,pomdt,64);
   // std::string cd = jiemi.GetDecoded();
   // std::cout<<cd<<std::endl;
  //  memcpy(te2.val,tes,64);
    for(int i = 0;i<8;i++){
        if(te2.val[i]!=pomd.val[i])
            std::cout<<"nononononono\n";
    }
    */
}