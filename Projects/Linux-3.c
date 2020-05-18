#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
#define N 5     //哲学家数量

#define LEFT(i)    (i+N-1)%N  //左手边哲学家编号
#define RIGHT(i)   (i+1)%N    //右手边哲家编号

#define HUNGRY    0     //饥饿
#define THINKING  1     //思考
#define EATING    2     //吃饭

#define U_SECOND 1000000   //1秒对应的微秒数
pthread_mutex_t mutex;     //互斥量

int state[N];  //记录每个哲学家状态
//每个哲学家的思考时间，吃饭时间，思考开始时间，吃饭开始时间
clock_t thinking_time[N], eating_time[N], start_eating_time[N], start_thinking_time[N];  
//线程函数
void *thread_function(void *arg);

int main()
{
    pthread_mutex_init(&mutex, NULL);
    
    pthread_t a,b,c,d,e;
    //为每一个哲学家开启一个线程，传递哲学家编号
    pthread_create(&a,NULL,thread_function,"0");
    pthread_create(&b,NULL,thread_function,"1");
    pthread_create(&c,NULL,thread_function,"2");
    pthread_create(&d,NULL,thread_function,"3");
    pthread_create(&e,NULL,thread_function,"4");
    //初始化随机数种子
    srand((unsigned int)(time(NULL)));
    while(1)
    {
        ;
    }
}

void *thread_function(void *arg)
{
    char *a = (char *)arg;
    int num = a[0] - '0';  //根据传递参数获取哲学家编号
    int rand_time; 
    while(1)
    {
        //关键代码加锁
        pthread_mutex_lock(&mutex);
        //如果该哲学家处于饥饿  并且  左右两位哲学家都没有在吃饭  就拿起叉子吃饭
        if(state[num] == HUNGRY && state[LEFT(num)] != EATING && state[RIGHT(num)] != EATING)
        {
            state[num] = EATING;
            start_eating_time[num] = clock(); //记录开始吃饭时间
            eating_time[num] = (rand() % 5 + 5) * U_SECOND;   //随机生成吃饭时间
            //输出状态
            printf("state: %d %d %d %d %d\n",state[0],state[1],state[2],state[3],state[4]);
            //printf("%d is eating\n",num);
        }
        else if(state[num] == EATING)
        {
            //吃饭时间已到 ，开始思考
            if(clock() - start_eating_time[num] >= eating_time[num])  //
            {
                state[num] = THINKING;
                //printf("%d is thinking\n",num);
                printf("state: %d %d %d %d %d\n",state[0],state[1],state[2],state[3],state[4]);
                start_thinking_time[num] = clock();  //记录开始思考时间
                thinking_time[num] = (rand() % 10 + 10) * U_SECOND;  //随机生成思考时间
            }
        }
        else if(state[num] == THINKING)
        {
            //思考一定时间后，哲学家饿了，需要吃饭
            if(clock() - start_thinking_time[num] >= thinking_time[num])
            {
                state[num] = HUNGRY;
                printf("state: %d %d %d %d %d\n",state[0],state[1],state[2],state[3],state[4]);
               // printf("%d is hungry\n",num);
            }
        }
        pthread_mutex_unlock(&mutex);       
    } 
}