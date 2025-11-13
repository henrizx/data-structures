#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 10

// Estrutura da fila

typedef struct
{
    int items[MAX];
    int frente;
    int tras;
    int tamanho;
} Fila;

void inicializador(Fila *f)
{
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

bool vazia(Fila *f)
{
    return f->tamanho == 0;
}

bool cheia(Fila *f)
{
    return f->tamanho == MAX;
}

bool enfileirar(Fila *f, int valor)
{
    if (cheia(f))
    {
        printf("Erro: Fila cheia!\n");
        return false;
    }

    f->tras = (f->tras + 1) % MAX;
    f->items[f->tras] = valor;
    f->tamanho++;
    printf("Enfileirado: %d\n", valor);
    return true;
}

int desenfileirar(Fila *f)
{
    if (vazia(f))
    {
        printf("Erro: fila vazia!\n");
        return -1;
    }

    int valor = f->items[f->frente];
    f->frente = (f->frente + 1) % MAX;
    f->tamanho--;
    printf("Desenfileirando: %d\n", valor);
    return valor;
}

int frente(Fila *f)
{
    if (vazia(f))
    {
        printf("Erro: Fila vazia!\n");
        return -1;
    }
    return f->items[f->frente];
}

void exibir(Fila *f)
{
    if (vazia(f))
    {
        printf("Fila Vazia!\n");
        return;
    }

    printf("Fila: [");
    for (int i = 0; i < f->tamanho; i++)
    {
        int idx = (f->frente + 1) % MAX;
        printf("%d", f->items[idx]);
        if (i < f->tamanho - 1)
            printf(", ");
    }
    printf("]\n");
}

int main()
{
    Fila fila;
    inicializador(&fila);

    printf("=== teste da fila === \n\n");

    printf("--- Enfileirando elementos --- \n");
    enfileirar(&fila, 10);
    enfileirar(&fila, 20);
    enfileirar(&fila, 30);
    enfileirar(&fila, 40);
    enfileirar(&fila, 50);

    printf("\n");
    exibir(&fila);

    printf("\n Elemento da frente: %d\n", frente(&fila));

    printf("\n --- Desenfileirando elementos ---\n");
    desenfileirar(&fila);
    desenfileirar(&fila);

    printf("\n");
    exibir(&fila);

    printf("\n ---Adicionando mais elementos --- \n");
    enfileirar(&fila, 60);
    enfileirar(&fila, 70);

    printf("\n");
    exibir(&fila);

    printf("\n --- Esvaziando a fila --- \n");
    while (!vazia(&fila))
    {
        desenfileirar(&fila);
    }

    printf("\n");
    exibir(&fila);

    printf("\n --- Testando desenfileirar a fila vazia");
    desenfileirar(&fila);

    return 0;
}