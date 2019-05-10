#include"lib.h"
#include"functions.cpp"
class ase{
    private:
        w keyword[44];
        void roxword(gf28 word[4][4],w keyword[4]){
            for(int i = 0;i<4;i++){
                for(int j = 0;j<4;j++){
                    word[i][j] = keyword[i].elem[j]+word[i][j];
                }
            }
        }
        void move(gf28 word[4][4]){
            gf28 temp[4];
            for(int i =1;i<4;i++){
                for(int j =0;j<4;j++){
                    temp[(4-i+j)%4] = word[i][j];
                }
                for(int q = 0;q<4;q++)
                word[i][q] = temp[q];
            }
        }
        void i_move(gf28 word[4][4]){
            gf28 temp[4];
            for(int i =1;i<4;i++){
                for(int j = 0;j<4;j++){
                    temp[(j+i)%4] = word[i][j];
                }
            for(int q = 0;q<4;q++)
                word[i][q] = temp[q];
       //      word[i] = temp;
            }
        }
        void col_mix(gf28 word[4][4]){
            gf28 r[4][4];
            gf28 temp[4][4];
            r[0][0] = 0x02;
            r[0][1] = 0x03;
            r[0][2] = 0x01;
            r[0][3] = 0x01;
            for(int i = 1;i<4;i++){
                for(int j = 0;j<4;j++){
                    r[i][j] = r[i-1][(j+3)%4];
                }
            }
            for(int i = 0;i<4;i++){
                for(int j = 0;j<4;j++){
                    temp[i][j] = (r[i][0]*word[0][j])+(r[i][1]*word[1][j])+(r[i][2]*word[2][j])+(r[i][3]*word[3][j]);
                }
            }
            for(int i =0;i<4;i++)
                for(int j = 0;j<4;j++)
                    word[i][j] = temp[i][j];

        }
        void i_col_mix(gf28 word[4][4]){
            gf28 temp[4][4];
            gf28 r[4][4];
            r[0][0] = 0x0e;
            r[0][1] = 0x0b;
            r[0][2] = 0x0d;
            r[0][3] = 0x09;
            for(int i = 1;i<4;i++){
                for(int j = 0;j<4;j++){
                    r[i][j] = r[i-1][(j+3)%4];
                }
            }
            for(int i = 0;i<4;i++){
                for(int j = 0;j<4;j++){
                    temp[i][j] = (r[i][0]*word[0][j])+(r[i][1]*word[1][j])+(r[i][2]*word[2][j])+(r[i][3]*word[3][j]);
                }
            }
            for(int i = 0;i<4;i++)
                for(int j = 0;j<4;j++)
                    word[i][j] = temp[i][j];

        }

    public:
        ase(w keyword[44]){
            for(int i = 0;i<44;i++)
                this->keyword[i] = keyword[i];
        }
        void jiami(unsigned char words[16]){
            gf28 temp[4][4];
            for(int i = 0;i<4;i++)
                for(int j = 0;j<4;j++){
                    temp[i][j] = words[i*4+j];
                }
         
          
          this->roxword(temp,this->keyword);
            for(int i = 1;i<=10;i++){
                for(int r = 0;r<4;r++)
                    for(int c = 0;c<4;c++)
                        temp[r][c] = s(temp[r][c]);
                this->move(temp);
                if(i<10)
                this->col_mix(temp);
                this->roxword(temp,this->keyword+i*4);
            }
       
            for(int i = 0;i<4;i++)
                for(int j = 0;j<4;j++)
                    words[i*4+j] = temp[i][j].getval();
        }
        void jiemi(unsigned char words[16]){
             gf28 temp[4][4];
            for(int i = 0;i<4;i++)
                for(int j = 0;j<4;j++){
                    temp[i][j] = words[i*4+j];
                }
               
           
           this->roxword(temp,this->keyword+40);
            for(int i = 9;i>=0;i--){
            this->i_move(temp);
          for(int r = 0;r<4;r++)
                   for(int c = 0;c<4;c++)
                        temp[r][c] = s_1(temp[r][c]);
           this->roxword(temp,this->keyword+i*4);
            if(i>0)
             this->i_col_mix(temp);
            }
        
            for(int i = 0;i<4;i++)
                for(int j = 0;j<4;j++)
                    words[i*4+j] = temp[i][j].getval();
        }

};
void inputkey(const char* a,w* keyword){
    for(int i = 0;i<4;i++)
        for(int j =0;j<4;j++)
            keyword[i].elem[j] = (unsigned char)a[i*4+j];
}
