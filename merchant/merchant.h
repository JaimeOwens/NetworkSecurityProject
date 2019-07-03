#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include<stdlib.h>
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
	uint64_t PIMD[8];
	char OI[SHORTLENGTH];
	int *DS;
	int CC[2];
	int sock_recv;
	int sock_send;
public:
	int StructChecker();
	int Receiver();
	// int CheckMessage();
	int Loader(unsigned char *);
	int Transformer(int, struct check_msg);
	// int Checker(char *);
	int Sender(char *);
};

int Merchant::Transformer(int dslen, struct check_msg chk_msg){
	char temp[SHORTLENGTH];
	memcpy(temp, chk_msg.PIMD, 128); 
//	cout<<"PIMD(content): "<<chk_msg.PIMD<<endl;
//	cout<<"PIMD(length): "<<strlen(chk_msg.PIMD)<<endl;
	memcpy(this->PIMD, chk_msg.PIMD, 128); 
	std::cout<<"PIMD\n";
	for(int i =0;i<8;i++)
		std::cout<<this->PIMD[i]<<std::endl;
	strcpy(this->OI, chk_msg.OI);
	std::cout<<this->OI<<std::endl;
	memcpy(this->DS, chk_msg.DS, dslen); 
	memcpy(this->CC, chk_msg.CC, 8);
	RSA rsa;
	rsa.SetPublicKey(1823347,549);
	rsa.SetPrivateKey(1823347,990349);
	rsa.decrypt(this->DS);
	std::string dsstr = rsa.GetDecoded();
	unsigned short dstemp[64];
	std::cout<<"ds\n";
	memcpy(dstemp,dsstr.data(),128);
	for(int i =0 ;i<64;i++){
		if((i+1)%4==0)
		std::cout<<dstemp[i]<<"\n";
		else
		{
			std::cout<<dstemp[i]<<" ";
		}
		
	}
	for(int i =0;i<8;i++)
		std::cout<<this->PIMD[i]<<std::endl;
	std::cout<<"CC   "<<this->CC[0]<<" "<<this->CC[1]<<std::endl;
}

int Merchant::StructChecker(){
	 
/* 	cout<<"MSG_A: "<<this->encrypt_msga<<endl;
	cout<<"MSG_B: "<<this->encrypt_msgb<<endl;
	cout<<"PIMD: "<<this->PIMD<<endl;
	cout<<"OI: "<<this->OI<<endl;
	cout<<"DS: "<<this->DS<<endl;
	cout<<"CC: "<<this->CC[0]<<' '<<this->CC[1]<<endl;
	int *p = this->DS;
   
    while(*p != 0){
        cout<<*p<<endl;
        p ++;
    }
	*/
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
	this->StructChecker();
}



// int Merchant::Checker(char *buffer){
// 	if(!CheckMessage()){
// 		sprintf(buffer, "%s", this->encrypt_msga);
// 	}
// 	else{
// 		sprintf(buffer, "%s", "counterfeit");
// 	}
// }

// int Merchant::CheckMessage(){
// 	RSA rsa;
// 	rsa.SetPublicKey(this->chk_msg->CC[0], this->chk_msg->CC[1]);
	
// 	int *buffer;
// 	buffer = (int *)&this->chk_msg->DS;
// 	rsa.decrypt(buffer);
// 	string decoded_temp = rsa.GetDecoded();
// 	char decoded[128];
// 	strcpy(decoded, decoded_temp.c_str());

// 	SHA512 T;
// 	hashval hashbuff;
// 	hashbuff = T.hash(this->chk_msg->OI);
// 	char OIMD[128];
// 	sprintf(OIMD, "%s", hashbuff.val);

// 	char POMD[128];
// 	int len = strlen(this->chk_msg->PIMD) > strlen(OIMD) ? strlen(this->chk_msg->PIMD) : strlen(OIMD);
// 	for(int i = 0; i< len; i++){
// 		POMD[i] = this->chk_msg->PIMD[i] | OIMD[i];
// 	}
// 	hashbuff = T.hash(POMD);
// 	memset(POMD, 0, sizeof(POMD));
// 	sprintf(POMD, "%s", hashbuff.val);

// 	if(strcmp(decoded,POMD) == 0){
// 		cout<<"RIGHT"<<endl;
// 		return 0;
// 	}
// 	else{
// 		cout<<"WRONG"<<endl;
// 		return -1;
// 	}
// }

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
			printf("accept()\n");
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
			// char buf2[1024];
			// memset(buf2, 0,sizeof(buf2));
			// this->Checker(buf2);
			// this->Sender(buf2);
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