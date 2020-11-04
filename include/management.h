#pragma once

struct pool{
	int size;
	int value[20000];
};

void AddToPidPool(int pid);

void ClearPid(int pid);

int GetPidPoolSize();
int GetPid(int num);
