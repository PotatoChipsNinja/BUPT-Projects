#include "header.hpp"

// 时钟信号发生器
void * timer(void *arg)
{
    INFO *info = (INFO *)arg;
    while(!info->state)
    {
        sleep(SECOND_PER_TIME_UNIT);
        //getchar();
        info->clock = true;
    }
    pthread_exit(0);
}

