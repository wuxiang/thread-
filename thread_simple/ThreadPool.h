/***************************************************************************/
/*文件名：ThreadPool.h
 *以下代码实现一个C++版的线程池
 *
 *
 *李海燕 2011-4-7
 * */
/***************************************************************************/
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdio.h>
#include <pthread.h>


typedef struct Pworker
{
	void* (*process) (void* arg);
	void* arg;
	struct Pworker* next;
}Thread_worker;

class ThreadPool
{
private:
	pthread_mutex_t queue_lock;
	pthread_cond_t  queue_ready;
	int max_thread_num;
	bool shutdown;
	pthread_t* threadid;
	Thread_worker* queue_head;
	int cur_queue_size;
public:
	ThreadPool(int max_thread_num);
	//线程池的初始化
	void pool_init();
	//线程池处理例程
	static void* thread_routine(void* arg);

	//接受任务(即向线程池中添加任务)
	int pool_add_worker(void* (*process)(void* arg),void* arg);
	//销毁线程池
	int pool_destroy();
	//获取线程池的最大活动线程数
	int get_max_thread_num();
	
};


#endif
