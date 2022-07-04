#include <stdio.h>

#include "arvore_b.h"

// Retorna cabecalho de arvore inicializado com valores padrão
cabecalho_arvore_t criar_cabecalho_arvore() {
    cabecalho_arvore_t cabecalho = {};
    cabecalho.status = '0'; // Inicialmente potencialmente inconsistente
    cabecalho.noRaiz = -1;
    cabecalho.proxRRN = 0;
    cabecalho.nroNos = 0;
}

// Retorna nó de arvore inicializado com valores padrão
no_arvore_t criar_no_arvore() {
    no_arvore_t no = {};
    no.tipoNo = FOLHA;
    no.nroChaves = 0;

    chave_t chave_vazia = {.id = -1, .RRN_dados = -1, .byteoffset_dados = -1};
    no.chaves[0] = chave_vazia;
    no.chaves[1] = chave_vazia;
    no.chaves[2] = chave_vazia;

    no.ponteiros[0] = -1;
    no.ponteiros[1] = -1;
    no.ponteiros[2] = -1;
    no.ponteiros[3] = -1;

    return no;
}

// Escreve cabelho em arquivo de árvore já aberto com permissão para escrita
void escrever_cabecalho_arvore(cabecalho_arvore_t cabecalho, FILE * arq_arvore) {

}

