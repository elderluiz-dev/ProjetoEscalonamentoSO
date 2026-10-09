#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "processes.h"

void fcfs(process_queue **queue)
{
    int max_procs = 0;
    process *actual = NULL;

    while(max_procs < 10 || (*queue)->p_count > 0)
    {   

        if(max_procs < 10)
        {
            add_process(queue);
            max_procs++;
        }

        if(actual == NULL)
        {
            process *aux = (*queue)->start;

            while (aux != NULL)
            {
                if (aux->arrival_time <= moment)
                {
                    if (actual == NULL ||
                        aux->arrival_time < actual->arrival_time)
                    {
                        actual = aux;
                    }
                }

                aux = aux->prox;
            }

        }

        if (actual != NULL)
        {
            printf("[%d] - Processing... | Clock: %lld | Arrival: %lld | Remaining: %lld\n",
            actual->pid,
            moment,
            actual->arrival_time,
            actual->remaining_time);

            cpu(actual);
        }

        tick();

        if(actual != NULL && actual->remaining_time == 0)
        {
            remove_process(queue, &actual);
            
        }
    }
}