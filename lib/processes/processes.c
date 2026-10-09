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

void add_process(process_queue **queue)
{

    if (queue == NULL || *queue == NULL)
    {
        printf("Falha aqui!\n");
        return;
    }

    process *proc = malloc(sizeof(*proc));
    if(proc == NULL){
        printf("Erro de alocação de memória em: add_process\n");
        return;
    }
    
    proc->pid = pid_count;
    proc->arrival_time = arrival_generator();
    proc->burst_time = rand() % 8 + 1;
    proc->remaining_time = proc->burst_time;
    proc->priority = rand() % 4;
    proc->prox = NULL;

    if((*queue)->start == NULL)
    {
        (*queue)->start = proc;
        (*queue)->end = proc;
    }else
    {
        (*queue)->end->prox = proc;
        (*queue)->end = proc;
    }

    (*queue)->p_count++;
    pid_count++;
}

void remove_process(process_queue **queue, process **proc)
{
    if (*queue == NULL || *proc == NULL)
    {
        return;
    }

    process *actual = (*queue)->start;
    process *prev = NULL;

    while (actual != NULL && actual != *proc)
    {
        prev = actual;
        actual = actual->prox;
    }

    if (actual == NULL)
    {
        return;
    }

    if (prev == NULL)
    {
        (*queue)->start = actual->prox;
    }else
    {
        prev->prox = actual->prox;
    }

    if((*queue)->end == actual)
    {
        (*queue)->end = prev;
    }

    free(actual);
    (*queue)->p_count--;

    *proc = NULL;

    return;
}
