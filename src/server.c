#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct serviceTable clientTable;

int clientfd = 0;

int ExeServer1(int port)
{
	int sockfd = 0, forClientSockfd = 0;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	clientTable.clientNum = 0;
	clientTable.clientMax = 50;
	clientTable.clientfd = (int*)malloc(sizeof(int) * clientTable.clientMax);

	printf("Start server1 at port:%d\n", port);

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

void WaitClientCommand(int clientfd, char* inputBuffer, int bufferLen)
{
	char message[] = {"% "};
	send(clientfd, message, sizeof(message), 0);

	recv(clientfd, inputBuffer, sizeof(char) * bufferLen, 0);
}	

int GetClientfd()
{
	return clientfd;
}

