/*************************************************************************************
*
*Projects Name:pthread-philosophers2-2
*
*
*************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
//筷子作为mutex
pthread_mutex_t chopstick[6] ;   //定义互斥锁
void *eat_think(void *arg)
{
	char phi = *(char *)arg;
	int left,right; //左右筷子的编号
	switch (phi){
		case 'A':
			left = 5;
			right = 1;
			break;
		case 'B':
			left = 1;
			right = 2;
			break;
		case 'C':
			left = 2;
			right = 3;
			break;
		case 'D':
			left = 3;
			right = 4;
			break;
		case 'E':
			left = 4;
			right = 5;
			break;
	}
 
	int i;
	for(;;){
		printf("哲学家%c正在思考问题\n",phi);
		sleep(5);
		//usleep(30000); //思考
		pthread_mutex_lock(&chopstick[left]); //拿起左手的筷子
		printf("哲学家%c饿了\n", phi);
		printf("哲学家%c拿起了%d号筷子（左）\n", phi, left);
		//方法1
		if (pthread_mutex_trylock(&chopstick[right]) == EBUSY){ //拿起右手的筷子	
			pthread_mutex_unlock(&chopstick[left]); //如果右边筷子被拿走放下左手的筷子
			printf("哲学家%c放下了%d号筷子（左）\n", phi, left);
			continue;
		}
/* 		//方法2
		int ret = pthread_mutex_trylock(&chopsticks[right]);
		if(ret != 0)
		{
			pthread_mutex_unlock(&chopsticks[left]);
			continue;
		} */
		
	    //	pthread_mutex_lock(&chopstick[right]); //拿起右手的筷子，如果想观察死锁，把上一句if注释掉，再把这一句的注释去掉
		printf("哲学家%c拿起了%d号筷子（右）\n", phi, right);
		printf("哲学家%c现在有两支筷子,开始进餐.\n",phi);
		sleep(5);
		//usleep(30000);  //吃饭
		pthread_mutex_unlock(&chopstick[left]); //放下左手的筷子
		printf("哲学家%c放下了%d号筷子（左）\n", phi, left);
		pthread_mutex_unlock(&chopstick[right]); //放下右手的筷子
		printf("哲学家%c放下了%d号筷子（右）\n", phi, right);
		printf("哲学家%c思考问题\n", phi);
 
	}
}
int main(){
	pthread_t A,B,C,D,E; //5个哲学家
 
	int i;
	for (i = 0; i < 5; i++)
		pthread_mutex_init(&chopstick[i],NULL);
	pthread_create(&A,NULL, eat_think, "A");
	pthread_create(&B,NULL, eat_think, "B");
	pthread_create(&C,NULL, eat_think, "C");
	pthread_create(&D,NULL, eat_think, "D");
	pthread_create(&E,NULL, eat_think, "E");
 
	pthread_join(A,NULL);
	pthread_join(B,NULL);
	pthread_join(C,NULL);
	pthread_join(D,NULL);
	pthread_join(E,NULL);
	return 0;
}