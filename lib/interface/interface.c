#include <stdio.h>
#include <stdlib.h>

#include "processes.h"
#include "cpu.h"
#include "fcfs.h"

int menu()
{
    int opt;

    printf("\n==== ESCALONADOR DE PROCESSOS ====\n");
    printf("1. First Come, First Served\n");
    printf("2. Shortest Job First\n");
    printf("3. Round Robin\n");
    printf("4. Priority Scheduling\n");
    printf("0. Sair\n");
    printf("> ");
    
    scanf("%d", &opt);
    return opt;
}

void clear_terminal(){
    system("clear");
}

void interface()
{
    process_queue *queue = NULL;

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
                fcfs(&queue);
                break;   
            }

            case 2:
            {
                clear_terminal();
                printf("SJF\n");
                break;
            }
            
            case 3:
            {
                clear_terminal();
                printf("RR\n");
                break;
            }

            case 4:
            {
                clear_terminal();
                printf("Priority\n");
                break;
            }

            case 0:
            {
                clear_terminal();
                printf("Programa encerrado pelo usuário.\n");
                return;
            }

            default:
            {
                clear_terminal();
                printf("Opção inválida.\n");
                break;
            }
        }        
    }

}