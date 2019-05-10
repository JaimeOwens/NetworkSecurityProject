#include"bitmartix.h"
bitmartix::bitmartix(int a ,int b){
            this->raw = a;
            this->col = b;
            this->elem = new std::bitset<1>*[a];
            for(int i = 0;i<a;i++){
                this->elem[i] = new std::bitset<1>[b];
            }

}
int bitmartix::craw(){
            return this->raw;
        }
int bitmartix::ccol(){
            return this->col;
        }
std::bitset<1> bitmartix::getelem(int i,int j){
            return this->elem[i][j];
        }
void bitmartix::set(int i,int j,bool temp){
            
            this->elem[i][j] = temp;
        }
void bitmartix::set(int i,int j,std::bitset<1>temp){
            this->elem[i][j] = temp;
        }
std::string bitmartix::makebits(){
            std::string temp="";
            for(int i=0;i<this->raw;i++)
                for(int j = 0;j<this->col;j++)
                    temp+=this->elem[i][j].to_string();
            return temp;
        }
void bitmartix::setrow(std::string rowval,int row){
            for(int i = 0;i<rowval.size();i++)
                if(rowval[i]=='0')
                    this->elem[row][i]=0;
                else
                {
                    this->elem[row][i]=1;
                }
                
        }
void bitmartix::setcol(std::string colstr,int col){
            for(int i = 0;i<colstr.size();i++)
                if(colstr[i]=='0')
                    this->elem[this->raw-1-i][col] = 0;
                else
                {
                    this->elem[this->raw-1-i][col] = 1;
                }
                
        }
