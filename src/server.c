#include "server.h"

#include "parser.h"
#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int serverNum = 0;
int clientfd = 0;
struct serviceTable clientTable = {
	.clientMax = 60,
	.clientSize = 0
};

int ExeServer1(int port)
{
	int sockfd = 0, forClientSockfd = 0;

	serverNum = 1;

	printf("Start server1 at port:%d\n", port);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1) printf("create socket failed\n");

	struct sockaddr_in serverInfo, clientInfo;
	socklen_t addrlen = sizeof(clientInfo);
	bzero(&serverInfo, sizeof(serverInfo));

	serverInfo.sin_family = PF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(port);
	bind(sockfd, (struct sockaddr *)&serverInfo, sizeof(serverInfo));
	listen(sockfd, 5);	

	while(1)
	{
		forClientSockfd = accept(sockfd, (struct sockaddr*)&clientInfo, &addrlen);

		printf("-----------------------------------------------------------\n");
		printf("                        New user login                     \n");
		printf("-----------------------------------------------------------\n");	

		pid_t pid = fork();
		
		if (pid != 0)
		{
			close(forClientSockfd);
		}
		else if (pid == 0) 
		{
			clientfd = forClientSockfd;

			dup2(clientfd, STDERR_FILENO);
			dup2(clientfd, STDOUT_FILENO);			

			return clientfd;
		}
	}
}

void ExeServer2(int port)
{
	int sockfd = 0, forClientSockfd = 0, sockMax = 0;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (serverNum == 0)
	{
		serverNum = 2;
		printf("Start server2 at port:%d\n", port);
	}

	if (sockfd == -1) printf("create socket failed\n");

	struct sockaddr_in serverInfo, clientInfo;
	socklen_t addrlen = sizeof(clientInfo);
	bzero(&serverInfo, sizeof(serverInfo));

	serverInfo.sin_family = PF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(port);
	bind(sockfd, (struct sockaddr *)&serverInfo, sizeof(serverInfo));
	listen(sockfd, clientTable.clientMax);

	while(1)
	{
		fd_set sockSet;
	
		FD_ZERO(&sockSet);
		FD_SET(sockfd, &sockSet);

		sockMax = sockfd;

		for(int i = 0; i < clientTable.clientSize; ++i)
		{
			FD_SET(clientTable.clientfds[i], &sockSet);
			if (sockMax < clientTable.clientfds[i]) sockMax = clientTable.clientfds[i]; 
		}
	
		select(sockMax + 1, &sockSet, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &sockSet))
		{
			forClientSockfd = accept(sockfd, (struct sockaddr *)&clientInfo, &addrlen);
			
			if (forClientSockfd < 0) printf("accept error\n");
			else
			{				
				printf("-----------------------------------------------------------\n");
				printf("                        New user login                     \n");	
				printf("-----------------------------------------------------------\n");
				SendLoginInfo(forClientSockfd, clientInfo);

				for(int i = 0; i <= clientTable.clientSize; ++i)
				{
					if (clientTable.clientfds[i] == 0)
					{
						clientTable.clientfds[i] = forClientSockfd;
						clientTable.clientInfo[i] = clientInfo;
						strcpy(clientTable.clientName[i], "no name");
			
						if (i == clientTable.clientSize) clientTable.clientSize++;
						
						break;
					}
				}
			}
		}

		for(int i = 0; i < clientTable.clientSize; ++i)
		{
			if (clientTable.clientfds[i] == 0) continue;

			if (FD_ISSET(clientTable.clientfds[i], &sockSet))
			{	
				clientfd = clientTable.clientfds[i];	
	
				int fd_err = dup(STDERR_FILENO);
				int fd_out = dup(STDOUT_FILENO);

				dup2(clientTable.clientfds[i], STDERR_FILENO);
				dup2(clientTable.clientfds[i], STDOUT_FILENO);
			
				ExeServer2Command();

				dup2(fd_err, STDERR_FILENO);
				dup2(fd_out, STDOUT_FILENO);

				close(fd_err);
				close(fd_out);
			}
		}
	}
}

void ExeServer2Command()
{
	int bufferLen = 16000;
	struct command input;

	char* buffer = (char*)calloc(bufferLen, sizeof(char));

	if (clientfd != 0)
	{
		recv(clientfd, buffer, sizeof(char) * bufferLen, 0);

		input = ParseCommand(buffer);
	
		if (input.tokenNumber != 0) Execute(input);

		send(clientfd, "% ", sizeof("% "), 0);

		memset(buffer, 0, bufferLen);
	}

	free(buffer);	
}

void ExeExitService()
{
	int index = GetIndexByClientfd(clientfd);
	
	clientTable.clientfds[index] = 0;
}

void SendLoginInfo(int clientfd, struct sockaddr_in clientInfo)
{
	char ipv4[20];

	inet_ntop(AF_INET, &clientInfo.sin_addr, ipv4, sizeof(struct sockaddr));

	int fd_old = dup(STDOUT_FILENO);

	dup2(clientfd, STDOUT_FILENO);

	printf("**************************************\n"
	       "** Welcom to the information server **\n" 
	       "**************************************\n");

	printf("*** User '(no name)' entered from %s:%d ***\n", ipv4, ntohs(clientInfo.sin_port));
	
	send(clientfd, "% ", sizeof("% "), 0);

	for(int i = 0; i < clientTable.clientSize; ++i)
	{
		if (clientTable.clientfds[i] == clientfd) continue;
		if (clientTable.clientfds[i] == 0) continue;			

		dup2(clientTable.clientfds[i], STDOUT_FILENO);
		
		printf("*** User '(no name)' entered from %s:%d ***\n", ipv4, ntohs(clientInfo.sin_port));
	}

	dup2(fd_old, STDOUT_FILENO);	
	close(fd_old);
}

void WaitClientCommand(int clientfd, char* inputBuffer, int bufferLen)
{
	send(clientfd, "% ", sizeof("% "), 0);

	recv(clientfd, inputBuffer, sizeof(char) * bufferLen, 0);
}	

int SetClientName(char* name)
{
	for(int i = 0; i < clientTable.clientSize; ++i)
	{
		if (strcmp(clientTable.clientName[i], name) == 0) return 0;
	}

	strcpy(clientTable.clientName[GetIndexByClientfd(clientfd)], name);
	return 1;
}

int GetServerNum()
{
	return serverNum;
}

int GetClientfd()
{
	return clientfd;
}

int GetClientSize()
{
	return clientTable.clientSize;
}

int GetIndexByClientfd(int fd)
{
	for(int i = 0; i < clientTable.clientSize; ++i)
	{
		if (clientTable.clientfds[i] == fd) return i;
	}
	return 0;
}

int* GetAllClientfd()
{
	return clientTable.clientfds;
}

char* GetClientName(int clientNum)
{
	return clientTable.clientName[clientNum];
}

struct sockaddr_in GetClientInfo(int clientNum)
{
	return clientTable.clientInfo[clientNum];
}
