/*************************************************************************************
*
*Projects Name:pthread-philosophers1-2
*思路：
*方案一
*仅当哲学家的左，右两只筷子均可用时，才允许他拿起筷子进餐
*
*通过互斥信号量 mutex 对哲学家进餐之前取左侧和右侧筷子的操作进行保护，
*可以防止死锁的出现。（当第i个哲学家将左右筷子都拿到了才允许其他哲学家拿筷子）
*如：哲学家4在拿起左筷子与右筷子中间没有其他的哲学家拿任何一个筷子，虽然他们也饿了，
*虽然他们要拿的筷子可能并没有被占用，所以这个会造成资源（筷子）得不到有效的利用。
*
*************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
 
#define N 5
 
sem_t chopsticks[N];//设置5种信号量，有5种不同类型的资源，每一种有1个,这样便于理解，因为每个哲学家需要的资源不同
 
pthread_mutex_t mutex;//定义互斥锁
 
int philosophers[N] = {0, 1, 2, 3, 4};//代表5个哲学家的编号
 
void delay (int len) {
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
 
void *philosopher (void* arg) {
	int i = *(int *)arg;
	int left = i;//左筷子的编号和哲学家的编号相同
	int right = (i + 1) % N;//右筷子的编号为哲学家编号+1
	while (1) {
		printf("哲学家%d正在思考问题\n", i);
		delay(60000);
		
		printf("哲学家%d饿了\n", i);
 
		pthread_mutex_lock(&mutex);//加锁
 
		sem_wait(&chopsticks[left]);//此时这个哲学家左筷子的信号量-1之后>=0时，表示能继续执行。
		printf("哲学家%d拿起了%d号筷子,现在只有一支筷子,不能进餐\n", i, left);
		sem_wait(&chopsticks[right]);
		printf("哲学家%d拿起了%d号筷子\n", i, right);
 
		pthread_mutex_unlock(&mutex);//解锁
 
		printf("哲学家%d现在有两支筷子,开始进餐\n", i);
		delay(60000);
		sem_post(&chopsticks[left]);
		printf("哲学家%d放下了%d号筷子\n", i, left);
		sem_post(&chopsticks[right]);
		printf("哲学家%d放下了%d号筷子\n", i, right);
	}
}
 
int main (int argc, char **argv) {
	srand(time(NULL));
	pthread_t philo[N];
	
	//信号量初始化
	for (int i=0; i<N; i++) {
		sem_init(&chopsticks[i], 0, 1);
	}
 
	pthread_mutex_init(&mutex,NULL);//初始化互斥锁
	
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
 
	pthread_mutex_destroy(&mutex);//销毁互斥锁
 
	return 0;
}
