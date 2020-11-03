#pragma once

#include <sys/types.h>

#include "parser.h"

struct pipeTable{
	int tableSize;
	int** lineCountTable;
};

void InitPipeTable(struct pipeTable *numberPipeTable, const int tableSize);
void FreePipeTable(struct pipeTable *numberPipeTable);

void Execute(struct command input, char* buffer);

char** CommandProcessing(struct command *input, char** oSeparation, char** oRedirection, int *oNumberPipe, int *processNum);
char** UserpipeProcessing(char** process, char* command, int processNum, int *readfd, int *userPipeIdx);

void AddNumberPipe(struct pipeTable *numberPipeTable, int fd, int number);
void UpdateNumberPipe(struct pipeTable *numberPipeTable, int *ofds);

int  ExeProcessPipe(char** process, int pastReadFd, char* numberPipeSeparation, int numberPipefd, int isHead);
void ExeProcessNumberPipe(char** process, int pastReadFd, struct pipeTable *numberPipeTable, int numberPipefd, char* separation, int line, int isHead);
void ExeProcessUserPipe(char** process, int pastReadFd, int numberPipefd, int userPipeIdx, int isHead);
void ExeProcess(char** process, int *pipefds, int infd, char* numberPipeSeparation, int numberPipefd, char* redirection, int isHead, int isTail);

int ExeBuiltInCommand(char** process);

void ExeExit();
void ExeName(char** process);
void ExeWho(char** process);
void ExeYell(char** process);
void ExeTell(char** process);
void ExeSetEnv(char** process);
void ExePrintEnv(char** process);

void ExeChild(char** process, int *pipefds, int infd, char* numberPipeSeparation, int numberPipefd, char* redirection, int isHead, int isTail);
void ExeParent(char** process, pid_t pid, int *pipefds, int infd, int isNumberPipe, int numberPipefd, int isTail);

void ExeRedirection(int *pipefds, int fd, char* redirection);

void ExePipe(char** process, int *pipefds, int infd, char* numberPipeSeparation, int isHead, int isTail);
void ExeNumberPipe(int numberPipefd);
void ExePipeHead(int *pipefds, char* numberPipeSeparation, int infd);
void ExePipeTail(int *pipefds, int infd);
void ExePipeMiddle(int *pipefds, char* numberPipeSeparation, int infd);

void ExeWait(pid_t pid);

void DoExecvp(char* process, char** arg);
