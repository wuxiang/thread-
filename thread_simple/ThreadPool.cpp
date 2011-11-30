#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include "ThreadPool.h"

using namespace std;

ThreadPool::ThreadPool(int max_thread_num):max_thread_num(max_thread_num){}

void ThreadPool::pool_init()
{
	pthread_mutex_init(&queue_lock, NULL);
	pthread_cond_init(&queue_ready, NULL);
	shutdown = false;
	queue_head = NULL;
	threadid = (pthread_t*)malloc(max_thread_num * sizeof(pthread_t*));
	cur_queue_size = 0;
	printf("==========================  max_thread_num = %d\n", max_thread_num);
	for(int i = 0; i < max_thread_num; i++)
	{
		pthread_create(&threadid[i],NULL,thread_routine,this);
	}
}


void* ThreadPool::thread_routine(void* arg)
{
	ThreadPool *pthis = (ThreadPool *)arg;
	printf("Starting thread 0x%x\n", pthread_self());
	while(1)
	{
		pthread_mutex_lock(&(pthis->queue_lock));
		while(pthis->cur_queue_size == 0 && pthis->shutdown == false)
		{
//			cout<<"thread "<<pthread_self()<<" is waiting"<<endl;
//			printf("+++++++++++++lock++++++++++++++++++++++++++!\n");
			printf("Thread 0x%x is waiting...\n", pthread_self());
			pthread_cond_wait(&(pthis->queue_ready), &(pthis->queue_lock));
		}

		if(pthis->shutdown == true)
		{
			printf("======================The pool will destroy!\n");
			pthread_mutex_unlock(&(pthis->queue_lock));//退出之前一定要先解锁
//			printf("======================unlock==============!\n");
			printf("Thread 0x%x will exit\n",pthread_self());
			pthread_exit(NULL);
			return NULL;
		}

		assert(pthis->cur_queue_size != 0);
		assert(pthis->queue_head != NULL);
		Thread_worker* head = NULL;
		head = pthis->queue_head;
		pthis->queue_head = (pthis->queue_head)->next;//将等待队列中的第一个任务取出来执行
		pthis->cur_queue_size--; //任务取出一个，等待队列就减1

		pthread_mutex_unlock(&(pthis->queue_lock));
//		printf("+++++++++++++unlock++++++++++++++++++++++++!\n");

		printf("Thread 0x%x is starting to work\n",pthread_self());

		(*(head->process))(head->arg);//调用自己的函数执行自己的任务
		printf("pthread 0x%x is complete=====================================================\n", pthread_self());
		free(head);
		head = NULL;
	}
	pthread_exit(NULL);
}


int ThreadPool::pool_add_worker(void* (*process)(void* arg),void* arg)
{
	printf("*********************************************Starting add task %d to pool......\n", *((int*)arg));
	Thread_worker* newworker = new Thread_worker;
	newworker->process = process;
	newworker->arg = arg;
	newworker->next = NULL;

	pthread_mutex_lock(&queue_lock);
	Thread_worker* TempWorker = queue_head;

	if(TempWorker != NULL)
	{
		while(TempWorker->next != NULL)
		{
			TempWorker = TempWorker->next;
		}
		TempWorker->next = newworker;
	}
	else
	{
		queue_head = newworker;
	}
	TempWorker = NULL;
	cur_queue_size++;
	pthread_mutex_unlock(&queue_lock);

	//唤醒一个空闲的线程来执行任务
	pthread_cond_signal(&queue_ready);
	printf("Awake a free thread to work!=================================================\n");
	return 0;
}


int ThreadPool::pool_destroy()
{
	if(shutdown == true) return -1;

	pthread_mutex_lock(&queue_lock);
	shutdown = true;

	/*唤醒所有的线程*/
	pthread_cond_broadcast(&queue_ready);
	pthread_mutex_unlock(&queue_lock);
	
	/*等待所有的线程退出*/
	for(int i = 0; i < max_thread_num; i++)
	{
		pthread_join(threadid[i], NULL);
	}

	/*********************************************************/
	printf("All the thread had exited!\n");
	printf("Releasing resource and space......\n");
	/*********************************************************/
	while(queue_head != NULL)
	{
		Thread_worker* head = queue_head;
		queue_head = queue_head->next;
		free(head);
	}

	/*销毁存放线程ID的空间*/
	free(threadid);

	/*销毁互斥量和信号量*/
	pthread_mutex_destroy(&queue_lock);
	pthread_cond_destroy(&queue_ready);

	printf("Okay,exit!\n");
	return 0;
}














