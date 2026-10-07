#pragma once

typedef struct process
{
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    struct process *prox;
} process;

typedef struct process_queue
{
    int p_count;
    process *start;
    process *end;
} process_queue;

long long clock();
void init_queue(process_queue **queue);
void add_process(process_queue **queue, process **proc);
void show_items(process_queue *queue);