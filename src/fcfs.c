// includes
#include <stdio.h>
#include <stdlib.h>

// headers
typedef struct process_node
{
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    struct process_node *prox;
}p_node;

typedef struct process_queue
{
    int p_count;
    p_node *start;
    p_node *end;
}p_queue;

void init_queue(p_queue **queue);
void add_node(p_queue *queue, int pid, int arriv, int burst);
void remove_node(p_queue *queue);
void disclaimer();
void insert_process(p_queue **queue, int *pid_count);
void fcfs(p_queue **queue);
void processor(p_queue **queue);

// functions
void init_queue(p_queue **queue)
{
    if (*queue != NULL) {
        printf("Fila já inicializada.\n");
        return;
    }

    *queue = malloc(sizeof(**queue));
    if (*queue == NULL) {
        printf("Falha na alocação de memória.\n");
        return;
    }

    (*queue)->p_count = 0;
    (*queue)->start = NULL;
    (*queue)->end = NULL;
}

void add_node(p_queue *queue, int proc_pid, int arriv, int burst)
{
    if(queue == NULL)
    {
        return;
    }
    
    p_node *new = malloc(sizeof(*new));
    if(new == NULL)
    {
        printf("Falha na alocação de memória.\n");
        return;
    }

    new->pid = proc_pid;
    new->arrival_time = arriv; 
    new->burst_time = burst;
    new->remaining_time = new->burst_time;
    new->prox = NULL;

    if(queue->p_count == 0)
    {
        queue->start = new;
        queue->end = new;
    }else
    {
        queue->end->prox = new;
        queue->end = new;
    }

    queue->p_count++;
    printf("Processo PID%d adicionado com sucesso!\n", proc_pid);
    return;
}

void remove_node(p_queue *queue)
{
    if (queue == NULL || queue->p_count == 0)
    {
        return;
    }

    p_node *rem = queue->start;
    queue->start = queue->start->prox;

    if((*queue).start == NULL)
    {
        queue->end = NULL;
    }

    free(rem);
    queue->p_count--;
    return;
}

void disclaimer()
{
    system("clear");

    printf("--- SIMULADOR FCFS ---\n");
    printf("\nEste é um simulador de fluxo unidirecional, passos anteriores não poderão ser repetidos, utilize com cuidado ou terá que reiniciar o processo.\n");

    printf("\nEste simulador funciona da seguinte forma: O usuário insere informações sobre cada processo (tempo de chegada e tempo de burst) e o escalonador executa cada processo conforme o TEMPO DE CHEGADA conforme a teoria First Come, First Served\n");

    printf("\nInsira qualquer NÚMERO para começar a inserir os processos\n> ");
    int temp;
    scanf("%d", &temp);

    return;
}

void insert_process(p_queue **queue, int *pid_count)
{
    char choose = 'a';
    int arrival;
    int burst;
    
    printf("\nTamanho atual da fila: %d\n", (*queue)->p_count);

    if((*queue)->p_count < 4)
    {
        printf("\nA fila deve ter pelo menos 4 processos para começar a simulação.\n");
    }else
    {
        printf("\nDeseja adicionar mais processos?(s/n) ");
        scanf(" %c", &choose);
    }

    if(choose == 'n')
    {
        return;
    }

    printf("\nPID: %d\n", *pid_count);
    printf("Tempo de chegada: ");
    scanf("%d", &arrival);
    printf("Tempo de burst: ");
    scanf("%d", &burst);

    system("clear");
    add_node(*queue, *pid_count, arrival, burst);

    (*pid_count)++;
    insert_process(queue, pid_count);
}

void processor(p_queue **queue)
{
    int clock = 0;
    int busy = 0;
    p_node *executing = NULL;

    while((*queue)->p_count > 0)
    {
        if(busy == 0)
        {
            p_node *aux = (*queue)->start;
            while(aux != NULL)
            {
                if(aux->arrival_time <= clock)
                {
                    if(executing == NULL || aux->arrival_time < executing->arrival_time)
                    {
                        executing = aux;
                        busy++;
                    }
                }
                aux = aux->prox;
            }
        }

        if(busy == 1)
        {
            printf("[%d] - Processing... | Clock: %d | Arrival: %d | Remaining: %d\n",
            executing->pid,
            clock,
            executing->arrival_time,
            executing->remaining_time);

            executing->remaining_time--;

            if(executing->remaining_time == 0)
            {
                remove_node(*queue);
                busy--;
            }
        }

        clock++;
    }

    return;
}

//main
int main()
{
    int pid_count = 0;

    p_queue *queue = NULL;
    init_queue(&queue);

    disclaimer();
    system("clear");
    insert_process(&queue, &pid_count);

    processor(&queue);

    return 0;
}
