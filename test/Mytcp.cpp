#include"Mytcp.h"
mytcp::mytcp(std::string add,int port){
    this->sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(add.data());
    if(connect(this->sockfd,(struct sockaddr*)&addr,sizeof(addr))<0){
        std::cout<<"erron to connect the server \n";
        close(this->sockfd);
        exit(1);
    }
}
int mytcp::mysend(unsigned char* msg,int len){
    return write(this->sockfd,msg,len);
}
int mytcp::myrecv(char* msg){
    return recv(this->sockfd,msg,4096,0);
}
mytcp::~mytcp(){
    close(this->sockfd);
}