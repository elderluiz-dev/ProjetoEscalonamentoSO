#include <stdlib.h>
#include <stdint.h>

typedef struct n_queue
{
    uint8_t data;
    n_queue *prox;
} n_queue;

typedef struct queue
{
    n_queue *start;
    n_queue end;
} queue;

void add_queue_node();
