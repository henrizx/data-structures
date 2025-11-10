package main

import (
	"fmt"
)

// Estrutura do nó
type No struct {
	valor   int
	proximo *No
}

// Estrutura da lista
type Lista struct {
	inicio *No
}

// Cria um novo nó
func criarNo(valor int) *No {
	return &No{valor: valor, proximo: nil}
}

// Insere no início da lista
func (l *Lista) InserirInicio(valor int) {
	novo := criarNo(valor)
	novo.proximo = l.inicio
	l.inicio = novo
}

// Insere no final da lista
func (l *Lista) InserirFinal(valor int) {
	novo := criarNo(valor)

	if l.inicio == nil {
		l.inicio = novo
		return
	}

	temp := l.inicio
	for temp.proximo != nil {
		temp = temp.proximo
	}
	temp.proximo = novo
}

// Remove um nó com o valor informado
func (l *Lista) Remover(valor int) {
	if l.inicio == nil {
		fmt.Println("Lista vazia.")
		return
	}

	// Caso o valor esteja no primeiro nó
	if l.inicio.valor == valor {
		l.inicio = l.inicio.proximo
		return
	}

	anterior := l.inicio
	temp := l.inicio.proximo

	for temp != nil && temp.valor != valor {
		anterior = temp
		temp = temp.proximo
	}

	if temp == nil {
		fmt.Printf("Valor %d nao encontrado.\n", valor)
		return
	}

	anterior.proximo = temp.proximo
}

// Exibe todos os elementos da lista
func (l *Lista) Exibir() {
	if l.inicio == nil {
		fmt.Println("Lista vazia.")
		return
	}

	temp := l.inicio
	fmt.Print("Lista: ")
	for temp != nil {
		fmt.Printf("%d ", temp.valor)
		temp = temp.proximo
	}
	fmt.Println()
}

// Limpa toda a lista
func (l *Lista) Limpar() {
	l.inicio = nil
	fmt.Println("Lista limpa da memoria.")
}

// Programa principal
func main() {
	lista := Lista{}

	lista.InserirFinal(10)
	lista.InserirFinal(20)
	lista.InserirFinal(30)
	lista.Exibir()

	lista.InserirInicio(5)
	lista.Exibir()

	lista.Remover(20)
	lista.Exibir()

	lista.Limpar()
	lista.Exibir()
}
