/*********************************************************************
*
*Projects Name:pthread-philosophers1-1
*思路：
*至多只允许四位哲学家同时去拿左筷子，最终能保证至少有一位哲学家能进餐，
*并在用完后释放两只筷子供他人使用。
*
**********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5


//信号量的数据类型为结构sem_t
sem_t chopsticks[N];//设置5种信号量，有5种不同类型的资源，每一种有1个,这样便于理解，因为每个哲学家需要的资源不同
sem_t m;//最多允许有m(4)个哲学家同时拿起左筷子
 
int philosophers[N] = {0, 1, 2, 3, 4};//代表5个哲学家的编号


//延时（哲学家思考），也可用usleep函数（毫秒级的阻塞）
void delay (int len) 
{
	int i = rand() % len;
	int x;
	while (i > 0) {
		x = rand() % len;
		while (x > 0) {
			x--;
		}
		i--;
	}
}
 
void *philosopher (void* arg) 
{
	int i = *(int *)arg;
	//左筷子的编号和哲学家的编号相同
	int left = i;
	//右筷子的编号为哲学家编号+1
	int right = (i + 1) % N;
	while (1) {
		printf("哲学家%d正在思考问题\n", i);
		delay(30000);
		
		printf("哲学家%d饿了\n", i);
		//如果前4个哲学家同时拿起左筷子，第五个不能同时拿起左筷子，保证至少有一位哲学家能吃到饭，解决（死锁状态）。
		sem_wait(&m);
		//此时这个哲学家左筷子的信号量-1之后>=0时，表示能继续执行。
		sem_wait(&chopsticks[left]);
		printf("哲学家%d拿起了%d号筷子,现在只有一支筷子,不能进餐\n", i, left);
		sem_wait(&chopsticks[right]);
		printf("哲学家%d拿起了%d号筷子, 现在有两支筷子,开始进餐\n", i, right);
		delay(30000);
		//sem_post是给信号量的值加上一个“1”，它是一个“原子操作”－－－即同时对同一个信号量做加“1”操作的两个线程是不会冲突的；
		//而同时对同一个文件进行读和写操作的两个程序就有可能会引起冲突
		sem_post(&chopsticks[left]);
		printf("哲学家%d放下了%d号筷子\n", i, left);
		//当哲学家释放了筷子时，信号量m+1
		sem_post(&m);
		sem_post(&chopsticks[right]);
		printf("哲学家%d放下了%d号筷子\n", i, right);

/* 		//当哲学家释放了左筷子时，信号量m+1
		sem_post(&m); */

		
	}
}
 
int main (int argc, char **argv)
{
	//srand函数是随机数发生器的初始化函数
	srand(time(NULL));
	pthread_t philo[N];
	
	//信号量初始化
	for (int i=0; i<N; i++) {
		sem_init(&chopsticks[i], 0, 1);
	}
	sem_init(&m, 0, 4);
	
	//创建线程
	for (int i=0; i<N; i++) {
		pthread_create(&philo[i], NULL, philosopher, &philosophers[i]);
	}
	
	//挂起线程
	for (int i=0; i<N; i++) {
		pthread_join(philo[i], NULL);
	}
	
	//销毁信号量
	for (int i=0; i<N; i++) {
		sem_destroy(&chopsticks[i]);
	}
	sem_destroy(&m);
 
	return 0;
}