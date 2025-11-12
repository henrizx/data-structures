#include <stdio.h>
#include <stdlib.h>

#define TAM 5

typedef struct
{
    int dados[TAM];
    int topo;
} Pilha;

void inicializar(Pilha *p)
{
    p->topo = -1;
}

int cheia(Pilha *p)
{
    return p->topo == TAM - 1;
}

int vazia(Pilha *p)
{
    return p->topo == -1;
}

void empilhar(Pilha *p, int valor)
{
    if (cheia(p))
    {
        printf("Pilha cheia!\n");
        return;
    }

    p->dados[++p->topo] = valor;
    printf("Empilhado: %d\n", valor);
}

int desempilhar(Pilha *p)
{
    if (vazia(p))
    {
        printf("Pilha vazia!\n");
        return -1;
    }

    int valor = p->dados[p->topo--];
    printf("Desempilhado: %d\n", valor);
    return valor;
}

int topo(Pilha *p)
{
    if (vazia(p))
    {
        printf("Pilha vazia!\n");
        return -1;
    }
    return p->dados[p->topo];
}

void exibir(Pilha *p)
{
    if (vazia(p))
    {
        printf("Pilha vazia!\n");
        return;
    }

    printf("Elementos na pilha: \n");
    for (int i = p->topo; i >= 0; i--)
        printf("%d\n", p->dados[i]);
}

int main()
{
    Pilha p;
    inicializar(&p);

    empilhar(&p, 10);
    empilhar(&p, 20);
    empilhar(&p, 30);

    exibir(&p);

    printf("Topo da pilha: %d\n", topo(&p));

    desempilhar(&p);
    exibir(&p);

    return 0;
}