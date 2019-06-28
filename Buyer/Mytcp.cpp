#include"Mytcp.h"
mytcp::mytcp(std::string add,int port){
    this->sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(add.data());
    connect(this->sockfd,(struct sockaddr*)&addr,sizeof(addr));
}
int mytcp::mysend(std::string msg){
    return send(this->sockfd,msg.data(),msg.size(),0);
}
int mytcp::myrecv(unsigned char* msg,int n){
    return recv(this->sockfd,msg,4096,0);
}
mytcp::~mytcp(){
    close(this->sockfd);
}