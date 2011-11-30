#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ThreadPool.h"

using namespace std;

void* myprocess1(void* arg)
{
	printf("Starting execute the task %d...\n",*((int*)arg));
	long long  i = 0;
	long long  sum = 0;
	for(i = 0; i < 1000000000; i++)
	{
		sum+=i;
	}
	cout << "sum = " << sum  << endl;
}

void* myprocess2(void* arg)
{
	printf("Starting execute the task %d...\n",*((int*)arg));
	cout << "=======================================" << endl;
	cout << "I am process 2---1!" << endl;
	cout << "I am process 2---2!" << endl;
	cout << "I am process 2---3!" << endl;
	cout << "=======================================" << endl;
}

int main()
{
	int task1 = 1;
	int task2 = 2;
	ThreadPool* pool = new ThreadPool(8);
	pool->pool_init();
	pool->pool_add_worker(myprocess1, (void*)&task1);
	pool->pool_add_worker(myprocess2, (void*)&task2);
//	sleep(3);
	pool->pool_destroy();

	return 0;
}
