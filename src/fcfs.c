// includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

typedef enum
{
    SUCCESS,
    ALOC_ERROR,
    NOT_INIT,
    NOT_ALLOWED
} ERROR_TYPE;

ERROR_TYPE init_queue(p_queue **queue);
ERROR_TYPE add_node(p_queue *queue, int pid, int arriv, int burst);
ERROR_TYPE remove_node(p_queue *queue);
ERROR_TYPE disclaimer();
ERROR_TYPE insert_process(p_queue **queue, int *pid_count);
ERROR_TYPE processor(p_queue **queue);

// functions
ERROR_TYPE init_queue(p_queue **queue)
{
    if (*queue != NULL) {
        printf("Fila já inicializada.\n");
        return 2;
    }

    *queue = malloc(sizeof(**queue));
    if (*queue == NULL) {
        return 1;
    }

    (*queue)->p_count = 0;
    (*queue)->start = NULL;
    (*queue)->end = NULL;

    return 0;
}

ERROR_TYPE add_node(p_queue *queue, int proc_pid, int arriv, int burst)
{
    if(queue == NULL)
    {
        return 2;
    }
    
    p_node *new = malloc(sizeof(*new));
    if(new == NULL)
    {
        printf("Falha na alocação de memória.\n");
        return 1;
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
        p_node *prev = NULL;
        p_node *actual = queue->start;
        while(actual != NULL && actual->arrival_time <= new->arrival_time)
        {
            prev = actual;
            actual = actual->prox;
        }

        if(prev == NULL)
        {
            queue->start = new;
        }else{
            prev->prox = new;
        }

        new->prox = actual;

        if(actual == NULL)
        {
            queue->end = new;
        }

    }

    queue->p_count++;
    return 0;
}

ERROR_TYPE remove_node(p_queue *queue)
{
    if (queue == NULL || queue->p_count == 0)
    {
        return 2;
    }

    p_node *rem = queue->start;
    queue->start = queue->start->prox;

    if((*queue).start == NULL)
    {
        queue->end = NULL;
    }

    free(rem);
    queue->p_count--;
    return 0;
}

ERROR_TYPE disclaimer()
{
    system("clear");

    printf("\033[1;36m");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                   FCFS SCHEDULER                         ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\033[0m");

    printf("\n╔══════════════════════════════════════════════════════════╗\n");
    printf("║  Este é um simulador de fluxo unidirecional, passos      ║\n");
    printf("║  anteriores não poderão ser repetidos, utilize com       ║\n");
    printf("║  cuidado ou terá que reiniciar o processo.               ║\n");
    printf("╠══════════════════════════════════════════════════════════╣\n");
    printf("║  Este simulador funciona da seguinte forma: o usuário    ║\n");
    printf("║  insere informações sobre cada processo (tempo de        ║\n");
    printf("║  chegada e tempo de burst) e o escalonador executa cada  ║\n");
    printf("║  processo conforme o TEMPO DE CHEGADA, baseando-se na    ║\n");
    printf("║  teoria First Come, First Served.                        ║\n");
    printf("╠══════════════════════════════════════════════════════════╣\n");
    printf("║  Inserir valores inválidos nos campos resultará no       ║\n");
    printf("║  encerramento do programa. Um processo não pode ser      ║\n");
    printf("║  iniciado num clock negativo (visto que ele começa no    ║\n");
    printf("║  próprio 0) e nenhum processo pode ter burst menor do    ║\n");
    printf("║  que 1, pois dessa forma ele nunca executaria.           ║\n");
    printf("╠══════════════════════════════════════════════════════════╣\n");
    printf("║  Insira qualquer tecla para começar a inserir os         ║\n");
    printf("║  processos                                               ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\n> ");

    while(getchar() != '\n');

    return 0;
}

ERROR_TYPE insert_process(p_queue **queue, int *pid_count)
{
    char choose = 's';
    int arrival;
    int burst;

    while(choose != 'n')
    {
        printf("\033[1;36m");
        printf("╔══════════════════════════════════════════════════════════╗\n");
        printf("║                  INSERIR PROCESSO                        ║\n");
        printf("╚══════════════════════════════════════════════════════════╝\n");
        printf("\033[0m");

        printf("╔══════════════════════════════════════════════════════════╗\n");
        printf("║  Tamanho atual da fila: %-2d                               ║\n", (*queue)->p_count);

        if((*queue)->p_count < 4)
        {
            printf("╠══════════════════════════════════════════════════════════╣\n");
            printf("║  A fila deve ter pelo menos 4 processos para começar     ║\n");
            printf("║  a simulação.                                            ║\n");
        }
        else
        {
            printf("╠══════════════════════════════════════════════════════════╣\n");
            printf("║  Deseja adicionar mais processos? (s/n)                  ║\n");
        }
        printf("╚══════════════════════════════════════════════════════════╝\n");

        if((*queue)->p_count >= 4)
        {
            printf("\n> ");
            scanf(" %c", &choose);
        }

        if(choose == 'n')
        {
            return 0;
        }
        if(choose != 's')
        {
            return 3;
        }

        printf("╔══════════════════════════════════════════════════════════╗\n");
        printf("║  PID: %-3d                                                ║\n", *pid_count);
        printf("╚══════════════════════════════════════════════════════════╝\n");

        printf("\nTempo de chegada: ");
        scanf("%d", &arrival);
        if(arrival < 0)
        {
            return 3;
        }

        printf("Tempo de burst: ");
        scanf("%d", &burst);
        if(burst <= 0)
        {
            return 3;
        }

        system("clear");

        printf("\033[1;36m");
        printf("╔══════════════════════════════════════════════════════════╗\n");
        if(add_node(*queue, *pid_count, arrival, burst) != 0)
        {
            printf("║  Falha na criação de processo. Programa encerrado.       ║\n");
            printf("╚══════════════════════════════════════════════════════════╝\n");
            printf("\033[0m");
            return 3;
        }
        printf("║  Processo PID %-3d adicionado com sucesso!                ║\n", *pid_count);
        printf("║  Chegada: %-3d  |  Burst: %-3d                             ║\n", arrival, burst);
        printf("╚══════════════════════════════════════════════════════════╝\n");
        printf("\033[0m");

        (*pid_count)++;
    }

    return 0;
}

ERROR_TYPE processor(p_queue **queue)
{
    int clock = 0;
    int busy = 0;
    p_node *executing = NULL;

    system("clear");
    printf("\033[1;36m");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                   FCFS SCHEDULER                         ║\n");
    printf("║══════════════════════════════════════════════════════════║\n");
    printf("\033[0m");

    while((*queue)->p_count > 0)
    {
        if(busy == 0)
        {
            if((*queue)->start->arrival_time <= clock)
            {
                executing = (*queue)->start;
                busy++;
                continue;
            }

            printf("\033[1;36m");
            printf("║  [IDLE] - Waiting...                      Clock: %-3d     ║\n", clock);
            printf("║══════════════════════════════════════════════════════════║\n");
            printf("\033[0m");
        }

        if(busy == 1)
        {
            printf("\033[1;36m");
            printf("║  [PID %-3d] - Processing...                Clock: %-3d     ║\n", executing->pid, clock);
            printf("║  Arrival:   %-3d                                          ║\n", executing->arrival_time);
            printf("║  Remaining: %-3d                                          ║\n", executing->remaining_time);
            printf("║══════════════════════════════════════════════════════════║\n");
            printf("\033[0m");

            executing->remaining_time--;

            if(executing->remaining_time == 0)
            {
                remove_node(*queue);
                busy--;
            }
        }
        
        clock++;
        sleep(1);
    }

    printf("\033[1;36m");
    printf("║                   TASKS FINISHED                         ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\033[0m");

    return 0;
}

//main
int main()
{
    int pid_count = 1;

    p_queue *queue = NULL;
    if(init_queue(&queue) != 0)
    {
        printf("\nFalha ao criar a fila. Programa encerrado.\n");
        return 1;
    }

    disclaimer();
    system("clear");
    
    if(insert_process(&queue, &pid_count) == 3)
    {
        printf("\nValor inválido inserido, programada encerrado.\n");
        return 1;
    }

    processor(&queue);
    free(queue);
    return 0;
}
