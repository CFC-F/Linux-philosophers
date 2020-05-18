#include<stdio.h>
#include<process.h>
#include<windows.h>
#include<stdlib.h>
#include<math.h>
HANDLE Mutext[5];
int chopstick = 0;
int *Physical;
unsigned int _stdcall ThreadFun(void *p)
{
	int* current = (int*)p;
	while (1)
	{
		if (abs(*current - chopstick)<4)
		{
			WaitForSingleObject(Mutext[*current], INFINITE);//加锁
			WaitForSingleObject(Mutext[(*current + 1) % 5], INFINITE);
			printf("哲学家%d正在就餐...\n", *current);
			printf("哲学家放下左边%d筷子\n", chopstick);
			chopstick = (chopstick + 1) % 5;
			printf("哲学家放下右边%d筷子\n\n", chopstick);
			Sleep(2000);
			ReleaseMutex(Mutext[(*current + 1) % 5]);//解锁
			ReleaseMutex(Mutext[(*current) % 5]);
		}
	}
	return 0;
}
int main()
{
	//初始化互斥变量，初始化要传递给线程的数组
	//HANDLE类型可以为（Event，Mutex，Process，Thread，Semaphore）数组
	int Physical[5];
	int i;
	for (i = 0; i<5; i++)
	{
		Mutext[i] = CreateMutexA(NULL, false, NULL);//创建了一个一个匿名的互斥锁创建后
		Physical[i] = i;
	}
	//然后开始进行创建线程
	HANDLE *hThread = NULL;
	hThread = new HANDLE[5];
	for (i = 0; i<5; i++)
	{
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, &Physical[i], 0, NULL);
	}
	WaitForMultipleObjects(5, hThread, true, INFINITE);//第二个参数为指定对象句柄组合中的第一个元素
	for (i = 0; i<5; i++)
	{
		CloseHandle(hThread[i]);//销毁创建的进程
		CloseHandle(Mutext[i]);//销毁锁
	}
	return 0;
}
