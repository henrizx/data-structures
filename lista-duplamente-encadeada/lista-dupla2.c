#include <stdio.h>
#include <stdlib.h>

// estrutura do nó
typedef struct No
{
    int dado;
    struct No *anterior;
    struct No *proximo;
} No;

// estrutura da lista

typedef struct
{
    No *cabeca;
    No *cauda;
    int tamanho;
} listaDupla;

/*
 inicialização e criação de nó
*/

// inicializa uma lista vazia
void inicializarLista(listaDupla *lista)
{
    lista->cabeca = NULL;
    lista->cauda = NULL;
    lista->tamanho = 0;
}

// criar um novo nó

No *criarNo(int valor)
{
    No *novoNo = (No *)malloc(sizeof(No));
    if (novoNo == NULL)
    {
        printf("ERRO: Memoria insuficiente!\n");
        return NULL;
    }

    novoNo->dado = valor;
    novoNo->anterior = NULL;
    novoNo->proximo = NULL;
    return novoNo;
}

/*Inserção no inicio*/

void inserirInicio(listaDupla *lista, int valor)
{
    No *novoNo = criarNo(valor);
    if (novoNo == NULL)
        return;

    if (lista->tamanho == 0)
    {
        // lista vazia: novo nó é tanto a cabeça quanto a cauda
        lista->cabeca = novoNo;
        lista->cauda = novoNo;
    }
    else
    {
        // lista não vazia: ajusta ponteiros
        novoNo->proximo = lista->cabeca;
        lista->cabeca->anterior = novoNo;
        lista->cabeca = novoNo;
    }

    lista->tamanho++;
}

/*inserir no final*/
void inserirFim(listaDupla *lista, int valor)
{
    No *novoNo = criarNo(valor);
    if (novoNo == NULL)
        return;

    if (lista->tamanho == 0)
    {
        // lista vazia
        lista->cabeca = novoNo;
        lista->cauda = novoNo;
    }
    else
    {
        // lista não vazia - ajustar ponteiros
        lista->cauda->proximo = novoNo;
        novoNo->anterior = lista->cauda;
        lista->cauda = novoNo;
    }
    lista->tamanho++;
}

/*Remoção do inicio*/

int removerInicio(listaDupla *lista)
{
    if (lista->tamanho == 0)
    {
        printf("ERRO: Lista vazia!\n");
        return -1;
    }

    No *noRemover = lista->cabeca;
    int valor = noRemover->dado;

    if (lista->tamanho == 1)
    {
        // unico nó da lista

        lista->cabeca = NULL;
        lista->cauda = NULL;
    }
    else
    {
        // Multiplos nós
        lista->cabeca = lista->cabeca->proximo;
        lista->cabeca->anterior = NULL;
    }

    free(noRemover);
    lista->tamanho--;
    return valor;
}

// remover final

int removerFim(listaDupla *lista)
{
    if (lista->tamanho == 0)
    {
        printf("ERRO: Lista vazia!\n");
        return -1;
    }

    No *noRemover = lista->cauda;
    int valor = noRemover->dado;

    if (lista->tamanho == 1)
    {
        // unicio nó da lista

        lista->cabeca = NULL;
        lista->cauda = NULL;
    }
    else
    {
        // multiplos nós
        lista->cauda = lista->cauda->anterior;
        lista->cauda->proximo = NULL;
    }

    free(noRemover);
    lista->tamanho--;
    return valor;
}

// Busca por valor

No *buscarValor(listaDupla *lista, int valor)
{
    No *atual = lista->cabeca;

    while (atual != NULL)
    {
        if (atual->dado == valor)
        {
            return atual;
        }

        atual = atual->proximo;
    }

    return NULL; // valor não encontrado
}

// inserir em posição especifica

void inserirPosicao(listaDupla *lista, int valor, int posicao)
{
    if (posicao < 0 || posicao > lista->tamanho)
    {
        printf("ERRO: Posicao invalida!\n");
        return;
    }

    if (posicao == 0)
    {
        inserirInicio(lista, valor);
        return;
    }

    // encontrar posicao
    No *atual = lista->cabeca;
    for (int i = 0; i < posicao; i++)
    {
        atual = atual->proximo;
    }

    // criar novo nó e ajustar ponteiros

    No *novoNo = criarNo(valor);
    novoNo->anterior = atual->anterior;
    novoNo->proximo = atual;
    atual->anterior->proximo = novoNo;
    atual->anterior = novoNo;

    lista->tamanho++;
}

// remoção por valor

int removerValor(listaDupla *lista, int valor)
{
    No *noRemover = buscarValor(lista, valor);

    if (noRemover == NULL)
    {
        printf("ERRO: valor não encontrado!\n");
        return -1;
    }

    // caso especiais
    if (noRemover == lista->cabeca)
    {
        return removerInicio(lista);
    }

    if (noRemover == lista->cauda)
    {
        return removerFim(lista);
    }

    // nó no meio da lista

    noRemover->anterior->proximo = noRemover->proximo;
    noRemover->proximo->anterior = noRemover->anterior;

    int valorRemovido = noRemover->dado;
    free(noRemover);
    lista->tamanho--;

    return valorRemovido;
}

// função de exibir

void exibirInicioFim(listaDupla *lista)
{
    printf("Lista (inicio -> fim): ");
    No *atual = lista->cabeca;
    while (atual != NULL)
    {
        printf("%d ", atual->dado);
        atual = atual->proximo;
    }
    printf("\n");
}

void exibirFimInicio(listaDupla *lista)
{
    printf("Lista (fim -> inicio): ");
    No *atual = lista->cauda;
    while (atual != NULL)
    {
        printf("%d ", atual->dado);
        atual = atual->anterior;
    }
    printf("\n");
}

void exibirInformacoes(listaDupla *lista)
{
    printf("Tamanho: %d\n", lista->tamanho);
    printf("Cabeca: %d\n", lista->cabeca ? lista->cabeca->dado : -1);
    printf("Cauda: %d\n", lista->cauda ? lista->cauda->dado : -1);
}

// destruir lista

void destruirLista(listaDupla *lista)
{
    No *atual = lista->cabeca;
    No *proximo;

    while (atual != NULL)
    {
        proximo = atual->proximo;
        free(atual);

        atual = proximo;
    }

    lista->cabeca = NULL;
    lista->cauda = NULL;
    lista->tamanho = 0;

    printf("Lista destruida!\n");
}

// main

int main()
{
    listaDupla lista;
    inicializarLista(&lista);

    printf("=== testando lista duplamente encadeada ===\n\n");

    // insercao

    printf("1. Inserindo valores: \n");
    inserirInicio(&lista, 10);
    inserirFim(&lista, 20);
    inserirInicio(&lista, 5);
    inserirFim(&lista, 30);
    inserirPosicao(&lista, 15, 2);

    exibirInicioFim(&lista);
    exibirFimInicio(&lista);
    exibirInformacoes(&lista);
    printf("\n");

    // busca

    printf("2. Buscando valores: \n");
    No *busca = buscarValor(&lista, 15);
    if (busca)
    {
        printf("Valor 15 encontrado\n");
    }

    busca = buscarValor(&lista, 99);
    if (!busca)
    {
        printf("valor 99 nao encontrado!\n");
    }

    printf("\n");

    // remocao

    printf("3. Removendo valores: \n");
    printf("removido do inicio: %d\n", removerInicio(&lista));
    printf("removido do fim %d\n", removerFim(&lista));
    printf("removido valor 15: %d\n", removerValor(&lista, 15));

    exibirInicioFim(&lista);
    exibirInformacoes(&lista);
    printf("\n");

    // limpeza final

    destruirLista(&lista);

    return 0;
}