/*************************************************************
*
*Projects Name:pthread-philosophers1-3
*思路：
*规定奇数号的哲学家先拿起他左边的筷子，然后再去拿他右边的筷子；
*而偶数号的哲学家则先拿起他右边的筷子，然后再去拿他左边的筷子。
*按此规定，将是1、2号哲学家竞争1号筷子，3、4号哲学家竞争3号筷子。
*即五个哲学家都竞争奇数号筷子，获得后，再去竞争偶数号筷子，
*最后总会有一个哲学家能获得两支筷子而进餐
*
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
 
#define N 5
 
sem_t chopsticks[N];//设置5种信号量，有5种不同类型的资源，每一种有1个,这样便于理解，因为每个哲学家需要的资源不同
 
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
		if(i % 2 == 0){
		printf("哲学家%d正在思考问题\n", i);
		delay(60000);
		
		printf("哲学家%d饿了\n", i);
		sem_wait(&chopsticks[right]);//此时这个哲学家左筷子的信号量-1之后>=0时，表示能继续执行。
		printf("哲学家%d拿起了%d号筷子,现在只有一支筷子,不能进餐\n", i, right);
		sem_wait(&chopsticks[left]);
		printf("哲学家%d拿起了%d号筷子, 现在有两支筷子,开始进餐\n", i, left);
		delay(60000);
		sem_post(&chopsticks[left]);
		printf("哲学家%d放下了%d号筷子\n", i, left);
		sem_post(&chopsticks[right]);
		printf("哲学家%d放下了%d号筷子\n", i, right);
		}
 
		else{
			printf("哲学家%d正在思考问题\n", i);
			delay(60000);
			
			printf("哲学家%d饿了\n", i);
			sem_wait(&chopsticks[left]);//此时这个哲学家左筷子的信号量-1之后>=0时，表示能继续执行。
			printf("哲学家%d拿起了%d号筷子,现在只有一支筷子,不能进餐\n", i, left);
			sem_wait(&chopsticks[right]);
			printf("哲学家%d拿起了%d号筷子, 现在有两支筷子,开始进餐\n", i, right);
			delay(60000);
			sem_post(&chopsticks[left]);
			printf("哲学家%d放下了%d号筷子\n", i, left);
			sem_post(&chopsticks[right]);
			printf("哲学家%d放下了%d号筷子\n", i, right);
		}
	}
}
 
int main (int argc, char **argv) {
	srand(time(NULL));
	pthread_t philo[N];
	
	//信号量初始化
	for (int i=0; i<N; i++) {
		sem_init(&chopsticks[i], 0, 1);
	}
	
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
 
	return 0;
}