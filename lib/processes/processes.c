#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "processes.h"
#include "interface.h"

int pid_count = 0;

void init_queue(process_queue **queue)
{
    if(*queue != NULL){
        return;
    }

    *queue = malloc(sizeof(**queue));
    if(*queue == NULL){
        printf("Erro de alocação de memória em: init_queue\n");
        return;
    }

    (*queue)->p_count = 0;
    (*queue)->start = NULL;
    (*queue)->end = NULL;
}

void add_process(process_queue **queue, process **proc)
{
    *proc = malloc(sizeof(**proc));
    if(*proc == NULL){
        printf("Erro de alocação de memória em: add_process\n");
        return;
    }
    
    (*proc)->pid = pid_count;
    (*proc)->arrival_time = arrival_generator();
    (*proc)->burst_time = rand() % 8 + 1;
    (*proc)->remaining_time = (*proc)->burst_time;
    (*proc)->priority = rand() % 4;

    if((*queue)->start == NULL)
    {
        (*queue)->start = *proc;
        (*queue)->end = *proc;
        (*queue)->p_count++;
        (*proc)->prox = NULL;
    }else
    {
        (*proc)->prox = (*queue)->end;
        (*queue)->end = *proc;
        (*queue)->p_count++;
    }
}

void show_items(process_queue *queue)
{
    if(queue == NULL)
    {
        printf("Vazio.\n");
        return;
    }

    printf("PID: %d\n", queue->end->pid);
    printf("Arrival time: %d\n", queue->end->arrival_time);
    printf("Burst time: %d\n", queue->end->burst_time);
    printf("Remaining time: %d\n", queue->end->remaining_time);
    printf("Priority: %d\n", queue->end->priority);

    return;
}

/*
int main()
{
    process_queue *queue;

    clear_terminal();
    while(1)
    {
        init_queue(&queue);
        int opt = menu();
        switch (opt)
        {
            case 1:
            {
                clear_terminal();
                process *proc;
                add_process(&queue, &proc);
                printf("Adicionado com sucesso.\n");
                break;   
            }

            case 2:
            {
                clear_terminal();
                show_items(queue);
                break;
            }

            case 0:
            {
                clear_terminal();
                printf("Programa encerrado pelo usuário.\n");
                return 0;
            }

            default:
            {
                clear_terminal();
                printf("Opção inválida.\n");
                break;
            }
        }        
    }

    return 0;
}
*/