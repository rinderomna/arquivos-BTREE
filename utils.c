#include <stdio.h>
#include <stdbool.h>

#include "utils.h"

// Retorna se o registro na posição 1 tem id maior do que o na posição 2
bool eh_maior(indice_t *indice, int pos1, int pos2) {
    registro_de_indice_t **registros = get_registros(indice);

    int id1 = get_id_registro_indice(registros[pos1]);
    int id2 = get_id_registro_indice(registros[pos2]);

    return id1 > id2;
}

// Troca posição de dois elementos
void troca_registros(indice_t *indice, int pos1, int pos2) {
    registro_de_indice_t **registros = get_registros(indice);

    registro_de_indice_t *temp = registros[pos1];
    registros[pos1] = registros[pos2];
    registros[pos2] = temp;
}

// Heapify a subárvore com raiz em i (índice do vetor)
// n é o tamanho da heap
void heapify(indice_t *indice, int n, int i) {
    // Acha o maior entre raiz, filho esquerdo e filho direito:

    // Inicializa o maior como a raiz
    int maior = i;

    // filho esquerdo = 2*i + 1
    int filho_da_esquerda = 2 * i + 1;

    // filho_da_direita = 2*i + 2
    int filho_da_direita = 2 * i + 2;

    // Se o filho da esquerda é maior do que a raiz
    if (filho_da_esquerda < n && eh_maior(indice, filho_da_esquerda, maior)) {
        maior = filho_da_esquerda;
    }

    // Se o filho da direita é maior do que o maior até aqui
    if (filho_da_direita < n && eh_maior(indice, filho_da_direita, maior)){
        maior = filho_da_direita;
    }

    // Troca e continua a fazer heapify se a raiz não é o maior
    if (maior != i) {
        troca_registros(indice, i, maior);

        // Heapify recursivamente a subárvore afetada
        heapify(indice, n, maior);
    }
}

// Função que executa o Heapsort nos registros de um índice em RAM
void Heapsort(indice_t *indice) {
    int n = get_n_registros(indice);
    
    // Construir Max Heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(indice, n, i);
    }

    // Heapsort
    for (int i = n - 1; i >= 0; i--) {
        troca_registros(indice, 0, i);

        // Heapify a partir da raiz para ter o maior elemento na raiz
        heapify(indice, i, 0);
    }
}