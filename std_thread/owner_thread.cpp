#include <stdio.h>

#include <thread>

void print()
{
    for (int i = 0; i < 20; ++i)
    {
        sleep(1);
        printf("thread 1\n");
    }
    printf("thread 1 over\n");
}

void print2()
{
    for (int i = 0; i < 20; ++i)
    {
        sleep(1);
        printf("thread 2\n");
    }
    printf("thread 2 over\n");
}


int main()
{
    //std::thread t1(print);
    //std::thread t2(print2);
    //std::thread t3;

    //t3 = std::move(t2);

    printf("start waiting in main thread\n");
    sleep(100);
    return 0;
}
