#include <iostream>
#include <thread>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//void print(const std::string& p)
//{
//    printf("print sleep start\n");
//    sleep(3);
//    printf("print sleep end\n");
//    for (int i =0; i < 10; ++i)
//    {
//        printf("%s\t", p.c_str());
//    }
//    printf("\n");
//    printf("print exit\n");
//}

void print(const char* p)
{
    printf("print sleep start\n");
    sleep(3);
    printf("print sleep end\n");
    for (int i =0; i < 10; ++i)
    {
        printf("%s\t", p);
    }
    printf("\n");
    printf("print exit\n");
}

void func1()
{
    printf("func1 start\n");
    char arr[] = "hello,world";
    char*  p = (char*)malloc(sizeof(arr));
    strcpy(p, arr);
    //std::string p(arr);
    //std::thread t(print, p);
    std::thread t(print, p);
    t.detach();
    printf("func1 exit\n");
}

int main()
{
    func1();
    sleep(30);
    return 0;
}

//here is compile style
//g++ internal_storage.cpp -omain -std=c++0x -g -static-libgcc -lpthread -L.
