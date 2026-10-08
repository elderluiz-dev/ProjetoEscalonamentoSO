#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "processes.h"

long long moment = 0;
long long arrival_generator()
{
    long long arriv_moment = moment;
    arriv_moment += (rand() % 4) + 1;
    return arriv_moment;
}

long long clock()
{
    moment++;
    sleep(1);
    return moment;
}

void cpu(process *p)
{
    clock();
    p->remaining_time--;
}