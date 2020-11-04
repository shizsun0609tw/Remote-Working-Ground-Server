#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

struct envPack{
	int envNum;
	char envName[100][1000];
	char envValue[100][1000];
};

struct serviceTable{
	int clientMax;
	int clientSize;
	int clientfds[60];
	char clientName[60][30];
	struct envPack clientEnv[60];
	struct sockaddr_in clientInfo[60];
};

struct userpipeTable{
	int pipeNum;
	int inIndex[3000];
	int outIndex[3000];
	int inPipe[3000];
	int outPipe[3000];
};

void RecycleProcess(int signo);

// return client fd
int ExeServer1(int port);
void ExeServer2(int port);

void ExeServer2Command();
void ExeExitService();

void PrintUserpipeOnServer();

void SendLoginInfo(int clientfd, struct sockaddr_in clientInfo);
void WaitClientCommand(int clientfd, char* inputBuffer, int bufferLen);

void Userpipefds(int user_idx);
void FreeUserpipefds(int pipe_idx);

void SetClientEnv();
void CleanClientEnv();
void SetEnv(char* envName, char* envValue);

int SetClientName(char* name);
int GetServerNum();
int GetClientfd();
int GetClientSize();
int GetIndexByClientfd(int fd);
int GetUserpipe(int pipe_idx, int *readfd);
int AddUserpipe(int pipe_idx);
int* GetUserpipefds(int pipe_idx);
int* GetAllClientfd();
char* GetClientName(int clientNum);
struct sockaddr_in GetClientInfo(int clientNum);
