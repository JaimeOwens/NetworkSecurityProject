#include "bank.h"


void bank::GetMsg(char* buffer)
{
	unsigned char s[256] = {0};
	rc4 rr;

	rr.rc4_setup(s, (unsigned char *)this->ks, strlen(this->ks)); 
	rr.rc4_decrypt(s, (unsigned char *)buffer, strlen(buffer));

	this->my_msg = (struct msg *)buffer;
}

void bank::GetKs()
{
	RSA rsa;
	KUc = this->chk_msg.CC;
	//rsa.SetPublicKey(KUc[0], KUc[1]);
	rsa.SetPrivateKey(4404199, 971);
	int *buffer;
	buffer = (int *)&this->chk_msg.EKS;
	rsa.decrypt(buffer);
	string decoded_temp = rsa.GetDecoded();
	this->ks = decoded_temp.c_str();
}
void bank::GetPIMD()
{
	SHA512 T;
	hashval hashbuff;
	hashbuff = T.hash(this->my_msg.PI);
	sprintf(this->PIMD, "%s", hashbuff.val);
}
void GetPOMD()
{
	RSA rsa;
	KUc = this->my_msg.CC;
	rsa.SetPublicKey(KUc[0], KUc[1]);
	int *buffer;
	buffer = (int *)&this->my_msg.DS;
	rsa.decrypt(buffer);
	string decoded_temp = rsa.GetDecoded();
	this->POMD = decoded_temp.c_str();
}

void GetMyPOMD()
{
	hashval hashbuff;
	char POMD[128];
	int len = strlen(this->PIMD) > strlen(this->my_msg.OIMD) ? strlen(this->PIMD) : strlen(this->my_msg.OIMD);
	for(int i = 0; i< len; i++){
		POMD[i] = this->PIMD[i] | this->my_msg.OIMD[i];
	}
	hashbuff = T.hash(POMD);
	memset(this->my_POMD, 0, sizeof(this->my_POMD));
	sprintf(this->my_POMD, "%s", hashbuff.val);
}


int bank::check()
{
    this->GetKs();
    this->GetPIMD();
    this->GetPOMD();
    this->GetMyPOMD();
	if(strcmp(this->my_POMD,this->POMD) == 0){
		return 0;
	}
	else{
		return -1;
	}
}

void bnak::Send()
{
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
	if (check() == -1)
	{
	    strcpy(buff, "Information mismatch");
	    buff[strlen(buff)-1]='\0';
	    write(this->sock_send, buff, sizeof(buff));
	}
	else
	{
		strcpy(buff, "Information confermed");
	    buff[strlen(buff)-1]='\0';
	    write(this->sock_send, buff, sizeof(buff));
	}
}

int bank::Receiver()
{
	this->sock_recv = socket(AF_INET,SOCK_STREAM,0);
	if(sock<0) {
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
				this->GetMsg(buf);
				this->Send();
				printf("wait...\n");
			}
			close(fd);
		} else if(fd>0) {
			close(fd);
		}
	}
}