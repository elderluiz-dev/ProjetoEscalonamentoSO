#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SLEEP_HALF_SECOND() usleep(500000)
#define MAX_PROC     20
#define MAX_ARRIVAL  10
#define MAX_BURST    10
#define MAX_PRIORITY 5
#define CLEAR_SCREEN "\033[H\033[2J"

typedef struct process_node
{
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    struct process_node *prox;
} p_node;

typedef struct process_queue
{
    int p_count;
    p_node *start;
    p_node *end;
} p_queue;

typedef struct
{   
    int arrival;
    int burst;
    int priority;
} p_input;

typedef struct
{
    int pid, arrival, priority, burst, completion, waiting, turnaround;
} p_result;

void queue_init(p_queue *q)
{
    q->p_count = 0;
    q->start = NULL;
    q->end = NULL;
}

p_node *node_create(int pid, int arrival, int burst, int priority) {
    p_node *n = malloc(sizeof(p_node));
    if (n == NULL)
    {
        printf("Erro: Falha na alocação de memória.\n");
        exit(1);
    }
    n->pid = pid;
    n->arrival_time = arrival;
    n->priority = priority;
    n->burst_time = burst;
    n->remaining_time = burst;
    n->priority = priority;
    n->prox = NULL;
    return n;
}

int less_arrival(const p_node *a, const p_node *b)
{
    if (a->arrival_time != b->arrival_time)
        return a->arrival_time < b->arrival_time;
    return a->arrival_time < b->arrival_time;
}

int more_priority(const p_node *a, const p_node *b)
{
    if (a->priority != b->priority)
        return a->priority > b->priority;
    if (a->arrival_time != b->arrival_time)
        return a->arrival_time < b->arrival_time;
    return a->pid < b->pid;
}

void insert_sorted(p_queue *q, p_node *node,
                          int (*less)(const p_node *, const p_node *))
{
    node->prox = NULL;

    if (q->start == NULL)
    {
        q->start = q->end = node;
    }
    else if (less(node, q->start))         
    {
        node->prox = q->start;
        q->start = node;
    }
    else if (!less(node, q->end))         
    {
        q->end->prox = node;
        q->end = node;
    }
    else                                 
    {
        p_node *cur = q->start;
        while (cur->prox != NULL && !less(node, cur->prox))
            cur = cur->prox;
        node->prox = cur->prox;
        cur->prox = node;
    }
    q->p_count++;
}

p_node *dequeue(p_queue *q)
{
    p_node *n = q->start;
    if (n == NULL)
        return NULL;

    q->start = n->prox;
    if (q->start == NULL)
        q->end = NULL;
    n->prox = NULL;
    q->p_count--;
    return n;
}

void queue_free(p_queue *q)
{
    p_node *n;
    while ((n = dequeue(q)) != NULL)
        free(n);
}

void flush_line()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int read_int(const char *label, int min, int max)
{
    int value, r;
    while(1)
    {
        printf("%s (%d a %d): ", label, min, max);
        r = scanf("%d", &value);
        if (r == EOF)
            exit(1);
        flush_line();
        if (r == 1 && value >= min && value <= max)
            return value;
        printf("Valor invalido.\n");
    }
}

int input_manual(p_input in[])
{
    char label[64];
    int i, n = read_int("Numero de processos", 1, MAX_PROC);

    for (i = 0; i < n; i++)
    {
        snprintf(label, sizeof label, "P%d - tempo de chegada", i + 1);
        in[i].arrival = read_int(label, 0, 999);
        snprintf(label, sizeof label, "P%d - tempo de execucao (burst)", i + 1);
        in[i].burst = read_int(label, 1, 99);
        snprintf(label, sizeof label, "P%d - nivel de prioridade", i + 1);
        in[i].priority = read_int(label, 1, 15);
    }

    return n;
}

int input_random(p_input in[])
{
    int i, seed, n = read_int("Numero de processos", 1, MAX_PROC);

    seed = read_int("Semente (0 = gerar automaticamente)", 0, 999999);
    if (seed == 0)
        seed = (int)(time(NULL) % 999999) + 1;
    srand((unsigned)seed);
    printf("Semente usada: %d (use-a para repetir esta simulacao)\n", seed);

    for (i = 0; i < n; i++)
    {
        in[i].arrival = rand() % (MAX_ARRIVAL + 1);
        in[i].burst = 1 + rand() % MAX_BURST;
        in[i].priority = 1 + rand() % MAX_PRIORITY;
    }
    return n;
}

void show_input(const p_input in[], int n)
{
    int i;
    printf("\nProcessos que serao escalonados:\n");
    printf("+-------+---------+-------+------------+\n");
    printf("| PID   | Chegada | Burst | Prioridade |\n");
    printf("+-------+---------+-------+------------+\n");
    for (i = 0; i < n; i++)
        printf("| P%-4d | %7d | %5d | %10d |\n", i + 1, in[i].arrival, in[i].burst, in[i].priority);
    printf("+-------+---------+-------+------------+\n");
}

void print_border(char ch)
{
    char *seg[] = {"------------", "-------", "--------------", "---------",
                        "--------", "----------", "--------------"};
    int i, k;
    printf("+");
    for (i = 0; i < 7; i++)
    {
        for (k = 0; seg[i][k] != '\0'; k++)
            putchar(ch);
        printf("+");
    }
    printf("\n");
}

void print_row(const char *state, const p_node *p, const char *progress)
{
    char pid[8] = "-", arr[8] = "-", bur[8] = "-", rem[8] = "-", pri[10] = "-";
 
    if (p != NULL)
    {
        snprintf(pid, sizeof pid, "P%d", p->pid);
        snprintf(arr, sizeof arr, "%d", p->arrival_time);
        snprintf(bur, sizeof bur, "%d", p->burst_time);
        snprintf(rem, sizeof rem, "%d", p->remaining_time);
        snprintf(pri, sizeof pri, "%d", p->priority);
    }
    
    printf("| %-10s | %-5s | %12s | %7s | %6s | %8s | %-12s |\n",
           state, pid, pri, arr, bur, rem, progress);
}

void render(int time, const p_queue *pending, const p_queue *ready,
                   const p_node *running, int finished, int total)
{
    const p_node *n;
    char bar[16];

    printf(CLEAR_SCREEN);
    printf("=== SIMULACAO PRIORITY ===    Tempo: %d\n\n", time);

    print_border('-');
    printf("| %-10s | %-5s | %12s | %7s | %6s | %8s | %-12s |\n",
           "Estado", "PID", "Prioridade", "Chegada", "Burst", "Restante", "Progresso");
    print_border('-');
    if (ready->start == NULL)
        print_row("(vazia)", NULL, "");
    for (n = ready->start; n != NULL; n = n->prox)
        print_row("Pronto", n, "");

    print_border('=');
    if (running != NULL)
    {
        int i, done = running->burst_time - running->remaining_time;
        int filled = done * 10 / running->burst_time;
        bar[0] = '[';
        for (i = 0; i < 10; i++)
            bar[i + 1] = (i < filled) ? '#' : '-';
        bar[11] = ']';
        bar[12] = '\0';
        print_row("Executando", running, bar);
    }
    else
    {
        print_row("CPU ociosa", NULL, "");
    }
    print_border('-');

    printf("\nAinda nao chegaram: %d | Concluidos: %d/%d\n",
           pending->p_count, finished, total);
}


void simulate(p_queue *pending, int total, p_result results[])
{
    p_queue ready;
    p_node *running = NULL;
    int time = 0, finished = 0;

    queue_init(&ready);

    while (finished < total)
    {
        while (pending->start != NULL && pending->start->arrival_time <= time)
            insert_sorted(&ready, dequeue(pending), more_priority);

        if (running == NULL && ready.p_count > 0)
            running = dequeue(&ready);

        render(time, pending, &ready, running, finished, total);
        SLEEP_HALF_SECOND();

        if (running != NULL)
        {
            running->remaining_time--;

            if (running->remaining_time == 0)
            {
                p_result *r = &results[finished++];
                r->priority = running->priority;
                r->pid = running->pid;
                r->arrival = running->arrival_time;
                r->burst = running->burst_time;
                r->completion = time + 1;
                r->turnaround = r->completion - r->arrival;
                r->waiting = r->turnaround - r->burst;
                free(running);
                running = NULL;
            }
        }
        time++;
    }

    render(time, pending, &ready, NULL, finished, total);
    queue_free(&ready);
}

void show_results(const p_result r[], int n)
{
    int i;
    float sum_wait = 0;

    printf("\nSIMULACAO CONCLUIDA (ordem de conclusao)\n");
    printf("+-------+--------------+-----------+---------+-----------+---------+\n");
    printf("| PID   |   Prioridade |   Chegada |   Burst | Conclusao | Espera  |\n");
    printf("+-------+--------------+-----------+---------+-----------+---------+\n");
    for (i = 0; i < n; i++)
    {
        printf("| P%-4d | %12d | %9d | %7d | %9d | %7d |\n",
               r[i].pid, r[i].priority, r[i].arrival, r[i].burst,
               r[i].completion, r[i].waiting);
        sum_wait += r[i].waiting;
    }
    printf("+-------+--------------+-----------+---------+-----------+---------+\n");
    printf("Tempo medio de espera:  %.2f\n", sum_wait / n);
}


int main() {
    p_queue pending;
    p_input in[MAX_PROC];
    p_result results[MAX_PROC];
    int mode, n, i;

    printf("==== ESCALONADOR DE PRIORIDADES ====\n");
    printf("1 - Entrada manual\n");
    printf("2 - Entrada aleatoria\n");
    mode = read_int("Escolha o modo", 1, 2);

    n = (mode == 1) ? input_manual(in) : input_random(in);

    show_input(in, n);
    printf("\nPressione Enter para iniciar a simulacao...");
    getchar();

    queue_init(&pending);
    for (i = 0; i < n; i++)
        insert_sorted(&pending, node_create(i + 1, in[i].arrival, in[i].burst,
                        in[i].priority), less_arrival);

    simulate(&pending, n, results);
    show_results(results, n);

    return 0;
}