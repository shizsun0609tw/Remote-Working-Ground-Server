#pragma once

struct serviceTable{
	int clientMax;
	int clientNum;
	int clientfds[60];
};

// return client fd
int ExeServer1(int port);
void ExeServer2(int port);

void ExeServer2Command(int clientfd);
void SendLoginInfo(int clientfd);
void WaitClientCommand(int clientfd, char* inputBuffer, int bufferLen);

int GetClientfd();
