#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

struct serviceTable{
	int clientMax;
	int clientNum;
	int clientfds[60];
};

// return client fd
int ExeServer1(int port);
void ExeServer2(int port);

void ExeServer2Command();
void ExeExitService();

void SendLoginInfo(int clientfd, struct sockaddr_in clientInfo);
void WaitClientCommand(int clientfd, char* inputBuffer, int bufferLen);

int GetServerNum();
int GetClientfd();
