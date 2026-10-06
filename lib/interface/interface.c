#include <stdio.h>
#include <stdlib.h>

int menu()
{
    int opt;

    printf("\n==== ESCALONADOR DE PROCESSOS ====\n");
    printf("1. First Come, First Served\n");
    printf("2. Shortest Job First\n");
    printf("3. Round Robin\n");
    printf("4. Priority Scheduling\n");
    printf("> \n");
    
    scanf("%d", &opt);
    return opt;
}

void clear_terminal(){
    system("clear");
}

void interface()
{
    clear_terminal();
    while(1)
    {
        int opt = menu();
        switch (opt)
        {
        case 1:
            printf("FCFS\n");
            break;
        
        default:
            printf("Opção inválida.\n");
            break;
        }        
    }

}