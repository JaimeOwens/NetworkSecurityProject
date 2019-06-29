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

#include "rsa.h"
#include "SHA512.h"
using namespace std;

#define SHORTLENGTH 128
#define LONGLENGTH 512
#define _RECVPORT_ 8886
#define _SENDPORT_ 8888
#define _BACKLOG_ 10

struct check_msg{
	char PIMD[SHORTLENGTH];
	char OI[SHORTLENGTH];
	char DS[SHORTLENGTH];
	int CC[2];
};

struct all_msg{
	int lena;
	int lenb;
	u_char *buffer;
};

class Merchant{
private:
	check_msg *chk_msg;
	char encrypt_msga[LONGLENGTH];
	char encrypt_msgb[LONGLENGTH];
	int sock_recv;
	int sock_send;
public:
	int CheckMessage();
	int Loader(char *);
	int Saver(char *, char *, char *);
	int Checker(char *);
	int PrepareSend();
	int Receiver();
	int Sender(char *);
};

int Merchant::Loader(char *buffer){
	all_msg *msg = (struct all_msg *)buffer;
	char *buffer_chk;
	char *buffer_a;
	char *buffer_b;
	memcpy(buffer_a, msg->buffer, msg->lena);
	memcpy(buffer_b, msg->buffer + msg->lena, msg->lenb);
	memcpy(buffer_chk, msg->buffer + msg->lena + msg->lenb, sizeof(check_msg));
	this->Saver(buffer_chk, buffer_a, buffer_b);
}

int Merchant::Saver(char *buffer_chk, char *buffer_a, char *buffer_b){
	this->chk_msg = (struct check_msg *)buffer_chk;
	strcpy(this->encrypt_msga, buffer_a);
	strcpy(this->encrypt_msgb, buffer_b);
}

int Merchant::Checker(char *buffer){
	if(!CheckMessage()){
		sprintf(buffer, "%s", encrypt_msga);
	}
	else{
		sprintf(buffer, "%s", "counterfeit");
	}
}

int Merchant::CheckMessage(){
	RSA rsa;
	rsa.SetPublicKey(this->chk_msg->CC[0], this->chk_msg->CC[1]);
	int *buffer;
	buffer = (int *)&this->chk_msg->DS;
	rsa.decrypt(buffer);
	string decoded_temp = rsa.GetDecoded();
	char decoded[128];
	strcpy(decoded, decoded_temp.c_str());

	SHA512 T;
	hashval hashbuff;
	hashbuff = T.hash(this->chk_msg->OI);
	char OIMD[128];
	sprintf(OIMD, "%s", hashbuff.val);

	char POMD[128];
	int len = strlen(this->chk_msg->PIMD) > strlen(OIMD) ? strlen(this->chk_msg->PIMD) : strlen(OIMD);
	for(int i = 0; i< len; i++){
		POMD[i] = this->chk_msg->PIMD[i] | OIMD[i];
	}
	hashbuff = T.hash(POMD);
	memset(POMD, 0, sizeof(POMD));
	sprintf(POMD, "%s", hashbuff.val);

	if(strcmp(decoded,POMD) == 0){
		return 0;
	}
	else{
		return -1;
	}
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
			while(1) {
				char buf[1024];
				memset(buf,'\0',sizeof(buf));
				read(client_sock,buf,sizeof(buf));
				this->Loader(buf);
				char buf2[1024];
				memset(buf2,'\0',sizeof(buf2));
				this->Checker(buf2);
				this->Sender(buf2);
				printf("wait...\n");
			}
			close(fd);
		} else if(fd>0) {
			close(fd);
		}
	}
}

int Merchant::PrepareSend(){
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
}

int Merchant::Sender(char *buffer){
	char buff[2048];
	memset(buff, 0, sizeof(buffer));
	strcpy(buff, buffer);
	buff[strlen(buff)-1]='\0';
	write(this->sock_send, buff, sizeof(buff));
	return 0;
}