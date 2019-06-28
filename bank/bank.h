#ifndef __BANK_H
#define __BANK_H 

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
#include "rc4.h"
using namespace std;

#define SHORTLENGTH 128
#define LONGLENGTH 512
#define _RECVPORT_ 6000
#define _SENDPORT_ 7000
#define _BACKLOG_ 10

struct msg
{
	char OIMD[SHORTLENGTH];
	char DS[SHORTLENGTH];
	int CC[2];
	char EKS[64];
	char PI[512];
};

class bank
{
private:
	char ks[128];
	char PIMD[128];
	char my_POMD[128];
    char POMD[128];
    msg *my_msg;
    int sock_recv;
	int sock_send;
public:
	bank();
	
    void GetMsg(char* buffer);
    void GetKs();
    void GetPIMD();
    void GetPOMD();
    void GetMyPOMD();
    int check();
    void Send();
    int Receiver();
	~bank();
	
};
#endif