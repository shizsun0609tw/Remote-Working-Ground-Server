#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

struct serviceTable{
	int clientMax;
	int clientSize;
	int clientfds[60];
	char clientName[60][30];
	struct sockaddr_in clientInfo[60];
};

// return client fd
int ExeServer1(int port);
void ExeServer2(int port);

void ExeServer2Command();
void ExeExitService();

void SendLoginInfo(int clientfd, struct sockaddr_in clientInfo);
void WaitClientCommand(int clientfd, char* inputBuffer, int bufferLen);

int SetClientName(char* name);
int GetServerNum();
int GetClientfd();
int GetClientSize();
int GetIndexByClientfd(int fd);
int* GetAllClientfd();
char* GetClientName(int clientNum);
struct sockaddr_in GetClientInfo(int clientNum);
