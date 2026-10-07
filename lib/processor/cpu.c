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

void cpu(process_queue **queue)
{
    process *atual = (*queue)->start;
    while((*atual).remaining_time > 0)
    {
        clock();
        (*atual).remaining_time--;
    }

    (*queue)->start = atual->prox;
    (*queue)->p_count--;

    if ((*queue)->p_count == 0)
        (*queue)->end = NULL;

    free(atual);
}