#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó na lista
typedef struct No
{
    int valor;
    struct No *proximo;
} No;

// função para criar um novo nó
No *criarNo(int valor)
{
    No *novo = (No *)malloc(sizeof(No));
    novo->valor = valor;
    novo->proximo = NULL;
    return novo;
}

// inserir no inicio
void inserirInicio(No **inicio, int valor)
{
    No *novo = criarNo(valor);
    novo->proximo = *inicio;
    *inicio = novo;
}

// função para inserir no final da lista

void inserirFinal(No **inicio, int valor)
{
    No *novo = criarNo(valor);

    if (*inicio == NULL)
    {
        *inicio = novo;
        return;
    }

    No *temp = *inicio;
    while (temp->proximo != NULL)
    {
        temp = temp->proximo;
    }

    temp->proximo = novo;
}

// função para remover um valor da lista

void remover(No **inicio, int valor)
{
    No *temp = *inicio;
    No *anterior = NULL;

    while (temp != NULL && temp->valor != valor)
    {
        anterior = temp;
        temp = temp->proximo;
    }

    if (temp == NULL)
    {
        printf("valor %d nao encontrado.\n", valor);
        return;
    }

    if (anterior == NULL)
    {
        *inicio = temp->proximo;
    }
    else
    {
        anterior->proximo = temp->proximo;
    }

    free(temp);
}

// Função para exibir os valores da lista

void exibir(No *inicio)
{
    No *temp = inicio;

    if (temp == NULL)
    {
        printf("Lista vazioa.\n");
        return;
    }

    printf("Lista: ");
    while (temp != NULL)
    {
        printf("%d ", temp->valor);
        temp = temp->proximo;
    }

    printf("\n");
}

// limpar lista

void limparLista(No **inicio)
{
    No *temp = *inicio;
    while (temp != NULL)
    {
        No *prox = temp->proximo;
        free(temp);
        temp = prox;
    }

    *inicio = NULL;
    printf("Lista limpa da memoria.\n");
}
// principal

int main()
{
    No *lista = NULL;

    inserirFinal(&lista, 10);
    inserirFinal(&lista, 20);
    inserirFinal(&lista, 30);
    exibir(lista);

    inserirInicio(&lista, 5);
    exibir(lista);

    remover(&lista, 20);
    exibir(lista);

    limparLista(&lista);
    exibir(lista);

    return 0;
}