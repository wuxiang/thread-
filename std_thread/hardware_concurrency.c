#include <thread>
#include <stdio.h>

int main()
{
    printf("%u\n", std::thread::hardware_concurrency());
    return 0;
}
