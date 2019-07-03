#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include "rsa.h"
#include "SHA512.h"
using namespace std;

#define SHORTLENGTH 128
#define MIDLENGTH 256
#define LONGLENGTH 512
#define START 8
#define _RECVPORT_ 8886
#define _SENDPORT_ 8888
#define _BACKLOG_ 10

struct check_msg{
	char PIMD[128];
	char OI[SHORTLENGTH];
	char DS[LONGLENGTH];
	int CC[2];
};

struct all_msg{
	int lena;
	int lenb;
	int lends;
	char buffer[2048];
};

class Merchant{
private:
	unsigned char *encrypt_msga;
	unsigned char *encrypt_msgb;
	// uint64_t PIMD[8];
	char PIMD[SHORTLENGTH];
	char OI[SHORTLENGTH];
	int *DS;
	int CC[2];
	int sock_recv;
	int sock_send;
public:
	int Receiver();
	int CheckMessage();
	int Loader(unsigned char *);
	int Transformer(int, struct check_msg);
	int Checker(char *);
	int Sender(char *);
};

int Merchant::Transformer(int dslen, struct check_msg chk_msg){
	char temp[SHORTLENGTH];
	memcpy(this->PIMD, chk_msg.PIMD, 128); 
	// for(int i =0;i<8;i++)
	// 	std::cout<<this->PIMD[i]<<std::endl;
	strcpy(this->OI, chk_msg.OI);
	memcpy(this->DS, chk_msg.DS, dslen); 
	memcpy(this->CC, chk_msg.CC, 8);
	RSA rsa;
	rsa.SetPrivateKey(this->CC[0],this->CC[1]);
	rsa.decrypt(this->DS);
	std::string dsstr = rsa.GetDecoded();
	unsigned short dstemp[64];
	std::cout<<"DS"<<endl;
	memcpy(dstemp,dsstr.data(),128);
	for(int i =0 ;i<64;i++){
		if((i+1)%4==0)
		std::cout<<dstemp[i]<<"\n";
		else{
			std::cout<<dstemp[i]<<" ";
		}
	}
	std::cout<<"CC   "<<this->CC[0]<<" "<<this->CC[1]<<std::endl;
}

int Merchant::Loader(unsigned char *buffer){
	check_msg chk_msg;
	check_msg *msg_p = &chk_msg;
	all_msg *msg = (struct all_msg *)buffer;
	std::cout<<&buffer<<std::endl;
	cout<<msg->lena<<' '<<msg->lenb<<' '<<msg->lends<<endl;
	this->encrypt_msga = new unsigned char [msg->lena+1];
	this->encrypt_msgb = new unsigned char [msg->lenb+1];
	this->DS = (int *)malloc(msg->lends);////这里有改动
	std::cout<<"len_ds is "<<msg->lends<<std::endl;
	memcpy(this->encrypt_msga, msg->buffer, msg->lena);
	std::cout<<&msg->buffer<<std::endl;
	std::cout<<msg->lena<<" lena "<<msg->lenb<<" lenb\n";
	memcpy(this->encrypt_msgb, msg->buffer + msg->lena, msg->lenb);
	memcpy(msg_p, msg->buffer + msg->lena + msg->lenb, sizeof(check_msg));
	this->Transformer(msg->lends, chk_msg);
}



int Merchant::Checker(char *buffer){
	if(!CheckMessage()){
		sprintf(buffer, "%s", this->encrypt_msga);
	}
	else{
		sprintf(buffer, "%s", "counterfeit");
	}
}

int Merchant::CheckMessage(){
	//get depressed DS
	RSA rsa;
	rsa.SetPrivateKey(this->CC[0],this->CC[1]);
	rsa.decrypt(this->DS);
	std::string dsstr = rsa.GetDecoded();
	unsigned short dstemp[64];
	memcpy(dstemp,dsstr.data(),128);

	//get OIMD
	string OI = this->OI;
	string OIMD = sha512(OI);
	
	//combine PIMD and POMD
	string PIMD = this->PIMD;
	char temp[128];
	for (int i = 0; i < 128; i++) {
		temp[i] = PIMD[i]|OIMD[i];
	}
	string POMD = sha512(temp, 128);
	unsigned short tds[64];
    memcpy(tds, POMD.data(), 128);
   	
   	cout<<"PIMD: "<<PIMD<<endl;
    cout<<"OIMD: "<<OIMD<<endl;
    cout<<"POMD: "<<endl;
    for(int i = 0;i<64;i++){
    	if((i+1)%4==0)
			std::cout<<tds[i]<<std::endl;
		else
			std::cout<<tds[i]<<" ";
    }
    cout<<"DS"<<endl;
    for(int i = 0;i<64;i++){
    	if((i+1)%4==0)
			std::cout<<dstemp[i]<<std::endl;
		else
			std::cout<<dstemp[i]<<" ";
    }
    //compare POMD and result of depressed DS
   	int flag = 0;
    for(int i = 0;i<64;i++){
    	if(tds[i] != dstemp[i]){
    		flag = -1;
    		break;
    	}
    }
    return flag;
}

int Merchant::Receiver(){
	this->sock_recv = socket(AF_INET,SOCK_STREAM,0);
	if(this->sock_recv<0) {
		printf("socket()\n");
	}
	struct sockaddr_in server_socket;
	struct sockaddr_in socket;
	bzero(&server_socket,sizeof(server_socket));
	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = htonl(INADDR_ANY);
	server_socket.sin_port = htons(_RECVPORT_);
	
	if(bind(this->sock_recv, (struct sockaddr*)&server_socket, sizeof(struct sockaddr_in))<0) {
		printf("bind()\n");
		close(this->sock_recv);
		return 1;
	}
	if(listen(this->sock_recv, _BACKLOG_)<0) {
		printf("listen()\n");
		close(this->sock_recv);
		return 2;
	}
	for(;;) {
		socklen_t len=0;
		int client_sock = accept(this->sock_recv, (struct sockaddr*)&socket, &len);
		if(client_sock<0) {
			// printf("accept()\n");
			return 3;
		}
		char buf_ip[INET_ADDRSTRLEN];
		memset(buf_ip,'\0',sizeof(buf_ip));
		inet_ntop(AF_INET,&socket.sin_addr,buf_ip,sizeof(buf_ip));
		printf("get connect\n");
		pid_t fd=fork();
		if(fd<0)
			printf("fork()\n");
		if(fd==0) {
			close(this->sock_recv);
			printf("port=%d,ip=%s\n",ntohs(socket.sin_port),buf_ip);
			unsigned char buf[2048];
			memset(buf,'\0',sizeof(buf));
			std::cout<<read(client_sock,buf,sizeof(buf))<<"  read size()\n";
			this->Loader(buf);
			char buf2[1024];
			memset(buf2, 0,sizeof(buf2));
			this->Checker(buf2);
			cout<<buf2<<endl;
			this->Sender(buf2);
			close(fd);
		} else if(fd>0) {
			close(fd);
		}
	}
}

int Merchant::Sender(char *buffer){
	this->sock_send = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server_sock;
	bzero(&server_sock, sizeof(server_sock));
	server_sock.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &server_sock.sin_addr);
	server_sock.sin_port = htons(_SENDPORT_);
	int ret = connect(this->sock_send, (struct sockaddr *)&server_sock, sizeof(server_sock));
	if(ret<0) {
		printf("Connect fail\n");
		return 1; 
	}
	printf("Connect success\n");
	char buff[2048];
	memset(buff, 0, sizeof(buffer));
	strcpy(buff, buffer);
	buff[strlen(buff)-1]='\0';
	write(this->sock_send, buff, sizeof(buff));
	return 0;
}