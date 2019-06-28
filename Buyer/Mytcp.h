#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<string>
#include<iostream>
class mytcp{
    private:
        int sockfd;
    public:
        mytcp(std::string add,int port);
        ~mytcp();
        int mysend(unsigned char * msg,int len);
        int myrecv(char* msg);
};
