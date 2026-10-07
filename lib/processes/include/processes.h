#pragma once

typedef struct process
{
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
} process;