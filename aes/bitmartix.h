
class bitmartix{
    private:
        std::bitset<1> ** elem;
        int col;
        int raw;
    public:
        bitmartix(int a,int b);
        bitmartix(){};
        int craw();
        int ccol();
        std::bitset<1> getelem(int i,int j);
        void set(int i,int j,bool temp);
         void set(int i,int j,std::bitset<1>temp);
        std::string makebits();
        void setrow(std::string rowval,int row);
        void setcol(std::string colstr,int col);
        friend std::ostream& operator<<(std::ostream& out, const bitmartix& a){
            std::string temp = "";
            for(int i = 0;i<a.raw;i++){
                for(int j = 0;j<a.col;j++){
                    temp+=a.elem[i][j].to_string();
                    temp+=" ";
                }
                temp+="\n";
            }
            out<<temp;
            return out;
        }
        bitmartix operator *(const bitmartix &b){
            bitmartix temp(this->raw,b.col);
            int n = this->col;
            std::bitset<1>elems ;
            for(int i = 0;i<this->raw;i++)
                for(int j = 0;j<b.col;j++){
                    elems = this->elem[i][0]&b.elem[0][j];
                    for(int r = 1;r<n;r++){
                        std::bitset<1> te = this->elem[i][r]&b.elem[r][j];
                        elems ^=  te;
                    }
                    temp.set(i,j,elems);
                    elems.reset();
                }
            return temp;
        }
        bitmartix operator +(const bitmartix &b){
            bitmartix temp(this->raw,this->col);
            for(int i = 0;i<this->raw;i++)
                for(int j = 0;j<this->col;j++)
                    temp.set(i,j,this->elem[i][j]^b.elem[i][j]);
            return temp;
        }
        std::string to_string(){
            int count = this->col*this->raw/8;//count of byte
            char * str = new char[count+1];
            std::string a = this->makebits();
            for(int i = 0;i<count;i++){
                std::bitset<8> temp;
                for(int j=0;j<8;j++){
                    temp.set(j,a[i*8+j]);
                }
                str[i] = (char)temp.to_ulong();
            }
             std::string s(str);
             delete []str;
             return s;
        }
        std::string to_bits(){
            
            std::string a ="";
            for(int i = 0;i<this->raw;i++){
                for(int j = 0;j<this->col;j++){
                    if(this->elem[i][j]==0)
                    a+="0";
                    else
                    {
                        a+="1";
                    }
                    
                }
                    
            }
            return a;
        }
};