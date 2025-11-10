#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó
typedef struct No
{
    int valor;
    struct No *anterior;
    struct No *proximo;
} No;

// Estrutura da lista
typedef struct
{
    No *inicio;
    No *fim;
} Lista;

// Cria uma nova lista vazia
void inicializarLista(Lista *lista)
{
    lista->inicio = NULL;
    lista->fim = NULL;
}

// Cria um novo nó
No *criarNo(int valor)
{
    No *novo = (No *)malloc(sizeof(No));
    novo->valor = valor;
    novo->anterior = NULL;
    novo->proximo = NULL;
    return novo;
}

// Insere no início
void inserirInicio(Lista *lista, int valor)
{
    No *novo = criarNo(valor);

    if (lista->inicio == NULL)
    {
        lista->inicio = novo;
        lista->fim = novo;
    }
    else
    {
        novo->proximo = lista->inicio;
        lista->inicio->anterior = novo;
        lista->inicio = novo;
    }
}

// Insere no final
void inserirFinal(Lista *lista, int valor)
{
    No *novo = criarNo(valor);

    if (lista->fim == NULL)
    {
        lista->inicio = novo;
        lista->fim = novo;
    }
    else
    {
        novo->anterior = lista->fim;
        lista->fim->proximo = novo;
        lista->fim = novo;
    }
}

// Remove um nó com o valor informado
void remover(Lista *lista, int valor)
{
    No *temp = lista->inicio;

    while (temp != NULL && temp->valor != valor)
    {
        temp = temp->proximo;
    }

    if (temp == NULL)
    {
        printf("Valor %d nao encontrado.\n", valor);
        return;
    }

    if (temp->anterior != NULL)
        temp->anterior->proximo = temp->proximo;
    else
        lista->inicio = temp->proximo;

    if (temp->proximo != NULL)
        temp->proximo->anterior = temp->anterior;
    else
        lista->fim = temp->anterior;

    free(temp);
}

// Exibe do início ao fim
void exibir(Lista *lista)
{
    No *temp = lista->inicio;

    if (temp == NULL)
    {
        printf("Lista vazia.\n");
        return;
    }

    printf("Lista (inicio → fim): ");
    while (temp != NULL)
    {
        printf("%d ", temp->valor);
        temp = temp->proximo;
    }
    printf("\n");
}

// Exibe do fim ao início
void exibirReverso(Lista *lista)
{
    No *temp = lista->fim;

    if (temp == NULL)
    {
        printf("Lista vazia.\n");
        return;
    }

    printf("Lista (fim → inicio): ");
    while (temp != NULL)
    {
        printf("%d ", temp->valor);
        temp = temp->anterior;
    }
    printf("\n");
}

// Libera todos os nós da memória
void limparLista(Lista *lista)
{
    No *temp = lista->inicio;

    while (temp != NULL)
    {
        No *prox = temp->proximo;
        free(temp);
        temp = prox;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    printf("Lista limpa da memoria.\n");
}

// Programa principal
int main()
{
    Lista lista;
    inicializarLista(&lista);

    inserirFinal(&lista, 10);
    inserirFinal(&lista, 20);
    inserirFinal(&lista, 30);
    exibir(&lista);
    exibirReverso(&lista);

    inserirInicio(&lista, 5);
    exibir(&lista);

    remover(&lista, 20);
    exibir(&lista);
    exibirReverso(&lista);

    limparLista(&lista);
    exibir(&lista);

    return 0;
}
