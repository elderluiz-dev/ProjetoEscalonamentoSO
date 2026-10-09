#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "processes.h"

long long moment = 0;
long long arrival_generator()
{
    long long arriv_moment = moment;
    arriv_moment += (rand() % 4) + 1;
    return arriv_moment;
}

void tick()
{
    moment++;
    // usleep(500000);
    return;
}

void cpu(process *p)
{
    if(p == NULL)
    {
        return;
    }

    if(p->remaining_time > 0)
    {
        p->remaining_time--;
    }

}