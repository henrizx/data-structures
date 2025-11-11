#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

/*
 * ESTRUTURA DE DADOS DO SEMÁFORO
 * O semáforo é representado pelo tipo sem_t da biblioteca POSIX
 * Internamente contém:
 * - Contador inteiro (valor do semáforo)
 * - Fila de threads bloqueadas
 */

// Recurso compartilhado (região crítica)
int contador_global = 0;

// Semáforos para diferentes propósitos
sem_t mutex;        // Semáforo binário para exclusão mútua
sem_t sem_contador; // Semáforo contador para múltiplos recursos

// Buffer compartilhado (Problema Produtor-Consumidor)
#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int in = 0;  // Índice de inserção
int out = 0; // Índice de remoção

// Semáforos para produtor-consumidor
sem_t vazio;        // Conta espaços vazios no buffer
sem_t cheio;        // Conta itens no buffer
sem_t mutex_buffer; // Protege acesso ao buffer

/*
 * ============================================
 * EXEMPLO 1: EXCLUSÃO MÚTUA SIMPLES
 * ============================================
 * Demonstra como usar semáforo binário (mutex)
 * para proteger região crítica
 */

void *thread_incrementa(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < 5; i++)
    {
        /*
         * sem_wait(&mutex): Operação P() ou down()
         * - Decrementa o semáforo
         * - Se valor < 0, bloqueia a thread
         * - Garante exclusão mútua
         */
        sem_wait(&mutex);

        // === REGIÃO CRÍTICA (só uma thread por vez) ===
        printf("Thread %d: Entrando na região crítica\n", id);
        int temp = contador_global;
        sleep(1); // Simula processamento
        temp++;
        contador_global = temp;
        printf("Thread %d: Contador = %d\n", id, contador_global);
        // === FIM DA REGIÃO CRÍTICA ===

        /*
         * sem_post(&mutex): Operação V() ou up()
         * - Incrementa o semáforo
         * - Se há threads bloqueadas, acorda uma delas
         * - Libera a região crítica
         */
        sem_post(&mutex);

        usleep(100000); // Pequena pausa entre iterações
    }

    return NULL;
}

/*
 * ============================================
 * EXEMPLO 2: PROBLEMA PRODUTOR-CONSUMIDOR
 * ============================================
 * Demonstra semáforos contador e sincronização
 */

void *produtor(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < 10; i++)
    {
        int item = rand() % 100; // Produz item aleatório

        /*
         * sem_wait(&vazio): Espera haver espaço no buffer
         * - Se buffer cheio, produtor bloqueia
         * - Decrementa contador de espaços vazios
         */
        sem_wait(&vazio);

        /*
         * sem_wait(&mutex_buffer): Protege acesso ao buffer
         * - Garante que apenas um thread modifica buffer por vez
         */
        sem_wait(&mutex_buffer);

        // === REGIÃO CRÍTICA - Adiciona ao buffer ===
        buffer[in] = item;
        printf("Produtor %d: Produziu %d na posição %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE; // Circular buffer
        // === FIM REGIÃO CRÍTICA ===

        sem_post(&mutex_buffer); // Libera acesso ao buffer

        /*
         * sem_post(&cheio): Sinaliza novo item disponível
         * - Incrementa contador de itens
         * - Acorda consumidor se estava esperando
         */
        sem_post(&cheio);

        usleep(500000); // Simula tempo de produção
    }

    return NULL;
}

void *consumidor(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < 10; i++)
    {
        /*
         * sem_wait(&cheio): Espera haver item no buffer
         * - Se buffer vazio, consumidor bloqueia
         * - Decrementa contador de itens
         */
        sem_wait(&cheio);

        sem_wait(&mutex_buffer); // Protege acesso ao buffer

        // === REGIÃO CRÍTICA - Remove do buffer ===
        int item = buffer[out];
        printf("Consumidor %d: Consumiu %d da posição %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE; // Circular buffer
        // === FIM REGIÃO CRÍTICA ===

        sem_post(&mutex_buffer); // Libera acesso ao buffer

        /*
         * sem_post(&vazio): Sinaliza espaço livre
         * - Incrementa contador de espaços vazios
         * - Acorda produtor se estava esperando
         */
        sem_post(&vazio);

        usleep(700000); // Simula tempo de consumo
    }

    return NULL;
}

/*
 * ============================================
 * EXEMPLO 3: SEMÁFORO CONTADOR (POOL DE RECURSOS)
 * ============================================
 * Simula pool de 3 impressoras compartilhadas
 */

#define NUM_IMPRESSORAS 3

void *usar_impressora(void *arg)
{
    int id = *(int *)arg;

    printf("Processo %d: Requisitando impressora...\n", id);

    /*
     * sem_wait(&sem_contador): Requisita recurso
     * - Se valor > 0: decrementa e continua (pega recurso)
     * - Se valor = 0: bloqueia até recurso ficar disponível
     */
    sem_wait(&sem_contador);

    printf("Processo %d: Usando impressora!\n", id);
    sleep(2); // Simula uso da impressora
    printf("Processo %d: Liberando impressora.\n", id);

    /*
     * sem_post(&sem_contador): Libera recurso
     * - Incrementa contador
     * - Acorda processo bloqueado se houver
     */
    sem_post(&sem_contador);

    return NULL;
}

/*
 * ============================================
 * FUNÇÃO PRINCIPAL - DEMONSTRAÇÃO
 * ============================================
 */

int main()
{
    printf("=== DEMONSTRAÇÃO DE SEMÁFOROS EM C ===\n\n");

    /*
     * ============================================
     * INICIALIZAÇÃO DOS SEMÁFOROS
     * ============================================
     * sem_init(semáforo, compartilhado, valor_inicial)
     * - semáforo: ponteiro para o semáforo
     * - compartilhado: 0 = threads, != 0 = processos
     * - valor_inicial: valor inicial do contador
     */

    // Semáforo binário (mutex): inicia com 1
    // Valor 1 = recurso disponível (desbloqueado)
    sem_init(&mutex, 0, 1);

    // Semáforo contador: inicia com 3 (3 impressoras)
    sem_init(&sem_contador, 0, NUM_IMPRESSORAS);

    // Semáforos produtor-consumidor
    sem_init(&vazio, 0, BUFFER_SIZE); // Buffer vazio = BUFFER_SIZE espaços
    sem_init(&cheio, 0, 0);           // Buffer vazio = 0 itens
    sem_init(&mutex_buffer, 0, 1);    // Mutex para buffer

    // ============================================
    // TESTE 1: EXCLUSÃO MÚTUA
    // ============================================
    printf("--- TESTE 1: EXCLUSÃO MÚTUA ---\n");
    pthread_t threads_mutex[3];
    int ids_mutex[3] = {1, 2, 3};

    for (int i = 0; i < 3; i++)
    {
        pthread_create(&threads_mutex[i], NULL, thread_incrementa, &ids_mutex[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads_mutex[i], NULL);
    }

    printf("\nValor final do contador: %d\n", contador_global);
    printf("(Esperado: 15 = 3 threads × 5 incrementos)\n\n");

    // ============================================
    // TESTE 2: PRODUTOR-CONSUMIDOR
    // ============================================
    printf("\n--- TESTE 2: PRODUTOR-CONSUMIDOR ---\n");
    pthread_t prod[2], cons[2];
    int ids_prod[2] = {1, 2};
    int ids_cons[2] = {1, 2};

    for (int i = 0; i < 2; i++)
    {
        pthread_create(&prod[i], NULL, produtor, &ids_prod[i]);
        pthread_create(&cons[i], NULL, consumidor, &ids_cons[i]);
    }

    for (int i = 0; i < 2; i++)
    {
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }

    // ============================================
    // TESTE 3: POOL DE RECURSOS
    // ============================================
    printf("\n--- TESTE 3: POOL DE IMPRESSORAS ---\n");
    pthread_t processos[5];
    int ids_proc[5] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&processos[i], NULL, usar_impressora, &ids_proc[i]);
        usleep(100000); // Espaça criação das threads
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(processos[i], NULL);
    }

    /*
     * ============================================
     * LIMPEZA - DESTRUIÇÃO DOS SEMÁFOROS
     * ============================================
     * sem_destroy(): Libera recursos do semáforo
     * Deve ser chamado quando semáforo não for mais usado
     */
    sem_destroy(&mutex);
    sem_destroy(&sem_contador);
    sem_destroy(&vazio);
    sem_destroy(&cheio);
    sem_destroy(&mutex_buffer);

    printf("\n=== PROGRAMA FINALIZADO ===\n");

    return 0;
}

/*
 * ============================================
 * COMPILAÇÃO E EXECUÇÃO
 * ============================================
 *
 * Para compilar no Linux:
 * gcc -o semaforo semaforo.c -lpthread
 *
 * Para executar:
 * ./semaforo
 *
 * ============================================
 * RESUMO DAS OPERAÇÕES
 * ============================================
 *
 * sem_init(sem, pshared, value):
 *   - Inicializa semáforo
 *   - pshared: 0 = threads, !=0 = processos
 *   - value: valor inicial do contador
 *
 * sem_wait(sem): [P() ou down()]
 *   - Decrementa semáforo
 *   - Se valor < 0, bloqueia thread
 *   - Operação atômica
 *
 * sem_post(sem): [V() ou up()]
 *   - Incrementa semáforo
 *   - Acorda thread bloqueada se houver
 *   - Operação atômica
 *
 * sem_destroy(sem):
 *   - Libera recursos do semáforo
 *   - Deve ser chamado ao final
 *
 * ============================================
 * TIPOS DE SEMÁFOROS IMPLEMENTADOS
 * ============================================
 *
 * 1. SEMÁFORO BINÁRIO (MUTEX):
 *    - Valor inicial: 1
 *    - Uso: Exclusão mútua
 *    - Garante: Apenas 1 thread na região crítica
 *
 * 2. SEMÁFORO CONTADOR:
 *    - Valor inicial: N (número de recursos)
 *    - Uso: Pool de recursos limitados
 *    - Garante: No máximo N threads usando recurso
 *
 * 3. SEMÁFOROS DE SINCRONIZAÇÃO:
 *    - Coordenam produtores e consumidores
 *    - 'vazio': conta espaços disponíveis
 *    - 'cheio': conta itens disponíveis
 *    - Previnem: buffer overflow e underflow
 */