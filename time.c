#include "philo.h"

unsigned int timestamp(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

void smart_sleep(philo_t *p,int time_ms)
{
    long long start = timestamp();
    while ((timestamp() - start) < time_ms && p->rules->stop != 1)
        usleep(500);
}
