#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server.h"
#include "parser.h"
#include "process.h"

void InitPath()
{
	setenv("PATH", "bin:.", 1);	
}

void ShellMainLoop(int port)
{
	while(1)
	{

		ExeServer2(port);
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

