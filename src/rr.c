#include <stdio.h>

// Estrutura para representar um processo
typedef struct {
    char nome[10];
    int tempo_execucao; // Tempo total necessário
    int tempo_restante; // Tempo que ainda falta rodar
} Processo;

void simulador_round_robin(Processo processos[], int n, int quantum) {
    int tempo_total = 0;
    int processos_concluidos = 0;
    
    printf("--- Iniciando Escalonamento (Quantum = %d) ---\n\n", quantum);
    
    // Loop continua até que todos os processos sejam concluídos
    while (processos_concluidos < n) {
        for (int i = 0; i < n; i++) {
            // Só processa se o processo ainda tiver tempo restante
            if (processos[i].tempo_restante > 0) {
                
                // Define quanto tempo o processo vai rodar nesta rodada
                int tempo_rodada = (processos[i].tempo_restante > quantum) ? quantum : processos[i].tempo_restante;
                
                printf("[Tempo %ds] Processo %s executando por %ds...\n", tempo_total, processos[i].nome, tempo_rodada);
                
                // Atualiza os tempos
                processos[i].tempo_restante -= tempo_rodada;
                tempo_total += tempo_rodada;
                
                // Verifica se o processo terminou
                if (processos[i].tempo_restante == 0) {
                    printf("  ✔ Processo %s CONCLUIDO no tempo %ds.\n", processos[i].nome, tempo_total);
                    processos_concluidos++;
                } else {
                    printf("  -> Processo %s interrompido (Falta %ds). Indo para o fim da fila.\n", processos[i].nome, processos[i].tempo_restante);
                }
            }
        }
    }
    
    printf("\n--- Todos os processos foram finalizados em %ds ---\n", tempo_total);
}

int main() {
    // Criando a lista de processos
    Processo lista_de_processos[] = {
        {"P1", 10, 10},
        {"P2", 4, 4},
        {"P3", 6, 6}
    };
    
    int num_processos = sizeof(lista_de_processos) / sizeof(lista_de_processos[0]);
    int quantum_definido = 3;
    
    // Executando a simulação
    simulador_round_robin(lista_de_processos, num_processos, quantum_definido);
    
    return 0;
}