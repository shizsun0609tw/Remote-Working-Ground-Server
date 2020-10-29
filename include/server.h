#pragma once

struct serviceTable {
	int clientMax;
	int clientNum;
	int* clientfd;
};

// return client fd
int ExeServer1(int port);

void WaitClientCommand(int clientfd, char* inputBuffer, int bufferLen);

int GetClientfd();
