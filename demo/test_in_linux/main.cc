#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#include <iostream>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* thread_task1(void* arg)
{
    pthread_mutex_lock(&mutex1);
    std::cout<<"-------thread_task1"<<std::endl;
    pthread_cond_wait(&cond,&mutex1);

    printf("thread_task1 start working\n");
    sleep(2);
    printf("thread_task1 works over\n");
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

void* thread_task2(void* arg)
{
    pthread_mutex_lock(&mutex2);
    std::cout<<"-------thread_task2"<<std::endl;
    pthread_cond_wait(&cond,&mutex2);

    printf("thread_task2 start working\n");
    sleep(2);
    printf("thread_task2 works over\n");
    pthread_mutex_unlock(&mutex2);

    return NULL;
}

void* broadcastDiffMutex(void* arg)
{
    // pthread_mutex_lock(&mutex1);
    // pthread_mutex_lock(&mutex2);
    pthread_cond_broadcast(&cond);
    pthread_cond_broadcast(&cond);
    // pthread_mutex_unlock(&mutex2);
    // pthread_mutex_unlock(&mutex1);
    return NULL;
}

void* signalDiffMutex(void* arg)
{
    pthread_cond_signal(&cond);
    return NULL;
}

int main()
{
    pthread_t thread_1,thread_2,thread_3;
    pthread_create(&thread_1,NULL,thread_task1,NULL);
    pthread_create(&thread_2,NULL,thread_task2,NULL);

    sleep(3);

    // pthread_create(&thread_3,NULL,broadcastDiffMutex,NULL);


// #ifdef SIGNAL
    pthread_create(&thread_3,NULL,signalDiffMutex,NULL);
    pthread_create(&thread_3,NULL,signalDiffMutex,NULL);
// #else
//     pthread_create(&thread_3,NULL,broadcastDiffMutex,NULL);
// #endif

    pthread_join(thread_1,NULL);
    pthread_join(thread_2,NULL);
    pthread_join(thread_3,NULL);

    return 0;

}

/**
使用同一个条件变量和不同互斥量时且使用signalDiffMutex唤醒线程时
-------thread_task2
-------thread_task1
thread_task2 start working
thread_task2 works over
原因分析：两个线程都sleep在等待cond，但是pthread_cond_signal只能唤醒一条线程


使用同一个条件变量和不同互斥量时且使用broadcastDiffMutex唤醒线程时
-------thread_task2
-------thread_task1
thread_task2 start working
thread_task2 works over

原因分析：这个结果比较怪异
 */ 