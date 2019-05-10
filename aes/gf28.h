class gf28{
    private:
        std::bitset<16> elem;
    public:
       
        gf28(char t);
        gf28(){this->elem.reset();}
        gf28(std::bitset<16>a);
        gf28 operator * (const gf28 & b);
        friend std::ostream& operator<<(std::ostream& out, const gf28& a){
            out<<a.elem;
            return out;
        }
        gf28  operator +(const gf28 &b){
            std::bitset<16> temp;
            temp = this->elem^b.elem;
            return gf28(temp);
        }
        gf28& operator = (const gf28 b){
            for(int i = 0;i<16;i++)
                this->elem = b.elem;
               return *this;
        }
        gf28& operator = (const int &b){
                this->elem = b;
               return *this;
        }       
        gf28& operator = (const std::string & b){
            std::string temp1 = b;
            
            for(int i = 0;i<4;i++){
                char a = temp1[i];
                temp1[i] = temp1[7-i];
                temp1[7-i] = a;
            }
            std::cout<<b<<std::endl;
            std::bitset<16> temp(temp1);
            this->elem = temp;
        }
        gf28& operator = (const char& b){
            this->elem = b;
        }
        gf28& operator =(const unsigned char & b){
            this->elem = b;
        }
        const gf28  operator !(){
            gf28 x(this->elem);
            gf28 temp = x;
            for(int i = 0;i<253;i++)    
                x = x*temp;
            return x;
        }
        unsigned int getval(){
            return this->elem.to_ulong();
        }
        std::string getstrval(){
            
            return this->elem.to_string().substr(8,16);
        }
        
};