#include "management.h"

struct pool pidPool = {
	.size = 0
};

void AddToPidPool(int pid)
{
	for (int i = 0; i <= pidPool.size; ++i)
	{
		if (pidPool.value[i] == 0)
		{
			pidPool.value[i] = pid;

			if (i == pidPool.size)
			{
				pidPool.size++;
			}

			return;
		}
	}
}

void ClearPid(int pid)
{
	for (int i = 0; i < pidPool.size; ++i)
	{
		if (pidPool.value[i] == 0) continue;

		if (pidPool.value[i] == pid)
		{
			pidPool.value[i] = 0;
		}
	}
}

int GetPidPoolSize()
{
	return pidPool.size;
}

int GetPid(int num)
{
	return pidPool.value[num];
}
