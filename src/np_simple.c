#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "server.h"
#include "parser.h"
#include "process.h"

void InitPath()
{
	setenv("PATH", "bin:.", 1);	
}

void ShellMainLoop(int port)
{
	int bufferLen = 16000;

	while(1)
	{
		struct command input;
		int clientfd = 0;
	
		clientfd = ExeServer1(port);
		
		char* buffer = (char*)calloc(bufferLen, sizeof(char));

		while (1)
		{
			WaitClientCommand(clientfd, buffer, bufferLen);
							
			input = ParseCommand(buffer);

			if (input.tokenNumber != 0) Execute(input, buffer);

			memset(buffer, 0, bufferLen);
		}

		free(buffer);
	}		
}

int main(int argc, char **argv, char **envp)
{
	int port = 8888;

	if (argc > 1) port = atoi(argv[1]);

	InitPath();

	ShellMainLoop(port);	

	return 0;
}

