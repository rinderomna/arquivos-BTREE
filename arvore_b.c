#include <stdio.h>
#include <stdlib.h>

#include "arvore_b.h"
#include "str.h"

// Retorna cabecalho de arvore inicializado com valores padrão
cabecalho_arvore_t criar_cabecalho_arvore() {
    cabecalho_arvore_t cabecalho = {};
    cabecalho.status = '0'; // Inicialmente potencialmente inconsistente
    cabecalho.noRaiz = -1;
    cabecalho.proxRRN = 0;
    cabecalho.nroNos = 0;

    return cabecalho;
}

// Retorna nó de arvore inicializado com valores padrão
no_arvore_t criar_no_arvore() {
    no_arvore_t no = {};
    no.RRN_arvore = -1; // Ainda Desconhecido
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
void escrever_cabecalho_arvore(cabecalho_arvore_t *cab_arvore, int tipo_do_arquivo, FILE *arq_arvore) {
    // Posicionar ponteiro no início do arquivo da árvore
    fseek(arq_arvore, 0, SEEK_SET);
    
    // Escrever status
    fwrite(&cab_arvore->status, 1, sizeof(char), arq_arvore);

    // Escrever nó raiz
    fwrite(&cab_arvore->noRaiz, 1, sizeof(int), arq_arvore);
    
    // Escrever próximo RRN
    fwrite(&cab_arvore->proxRRN, 1, sizeof(int), arq_arvore);
    
    // Escrever número de nós
    fwrite(&cab_arvore->nroNos, 1, sizeof(int), arq_arvore);

    // Completar com lixo
    char *lixo = NULL;
    
    if (tipo_do_arquivo == 1) {
        lixo = cria_lixo(TAM_REG1_ARV - 13);
        fwrite(lixo, TAM_REG1_ARV - 13, sizeof(char), arq_arvore);
    } else if (tipo_do_arquivo == 2) {
        lixo = cria_lixo(TAM_REG2_ARV - 13);
        fwrite(lixo, TAM_REG2_ARV - 13, sizeof(char), arq_arvore);
    }

    free(lixo);
}

// Lê cabecalho de arquivo de árvore já aberto com permissão para leitura
cabecalho_arvore_t ler_cabecalho_arvore(FILE *arq_arvore, int tipo_do_arquivo) {
    cabecalho_arvore_t cab_arvore = {};

    // Posicionar ponteiro no início do arquivo da árvore
    fseek(arq_arvore, 0, SEEK_SET);

    // Escrever status
    fread(&cab_arvore.status, 1, sizeof(char), arq_arvore);

    // Escrever nó raiz
    fread(&cab_arvore.noRaiz, 1, sizeof(int), arq_arvore);
    
    // Escrever próximo RRN
    fread(&cab_arvore.proxRRN, 1, sizeof(int), arq_arvore);
    
    // Escrever número de nós
    fread(&cab_arvore.nroNos, 1, sizeof(int), arq_arvore);

    // Pular lixo
    int offset = (tipo_do_arquivo == 1) ? TAM_REG1_ARV - 13 : TAM_REG2_ARV - 13;
    fseek(arq_arvore, offset, SEEK_CUR);

    return cab_arvore;
}

// Escreve nó em arquivo de árvore de tipo indicado já aberto com permissão para leitura
void escrever_no_arvore(no_arvore_t *no, int tipo_do_arquivo, FILE *arq_arvore) {
    // Escrever tipo do nó:
    fwrite(&no->tipoNo, 1, sizeof(char), arq_arvore);

    // Escrever número de chaves:
    fwrite(&no->nroChaves, 1, sizeof(int), arq_arvore);

    // Escrever chaves segundo tipo do arquivos:
    for (int i = 0; i < 3; i++) {
        fwrite(&no->chaves[i].id, 1, sizeof(int), arq_arvore);
        if (tipo_do_arquivo == 1) {
            fwrite(&no->chaves[i].RRN_dados, 1, sizeof(int), arq_arvore);
        } else if (tipo_do_arquivo == 2) {
            fwrite(&no->chaves[i].byteoffset_dados, 1, sizeof(long long int), arq_arvore);
        }
    }

    // Escrever ponteiros para nós descendentes:
    for (int i = 0; i < 4; i++) {
        fwrite(&no->ponteiros[i], 1, sizeof(int), arq_arvore);
    }
}

// Lê nó de arquivo de árvore de tipo indicado já aberto com permissão para leitura
no_arvore_t ler_no_arvore(int tipo_do_arquivo, FILE *arq_arvore) {
    no_arvore_t no = {};

    // Ler tipo do nó:
    fread(&no.tipoNo, 1, sizeof(char), arq_arvore);

    // Ler número de chaves:
    fread(&no.nroChaves, 1, sizeof(int), arq_arvore);

    // Ler chaves segundo tipo do arquivos:
    for (int i = 0; i < 3; i++) {
        fread(&no.chaves[i].id, 1, sizeof(int), arq_arvore);
        if (tipo_do_arquivo == 1) {
            fread(&no.chaves[i].RRN_dados, 1, sizeof(int), arq_arvore);
        } else if (tipo_do_arquivo == 2) {
            fread(&no.chaves[i].byteoffset_dados, 1, sizeof(long long int), arq_arvore);
        }
    }

    // Ler ponteiros para nós descendentes:
    for (int i = 0; i < 4; i++) {
        fread(&no.ponteiros[i], 1, sizeof(int), arq_arvore);
    }

    return no;
}

// Posiciona ponteiro do arquivo de árvore de tipo indicado no rrn indicado
void posicionar_em_rrn_arvore(FILE *arq_arvore, int tipo_do_arquivo, int rrn) {
    long long int byteoffset = 0;

    if (tipo_do_arquivo == 1) {
        byteoffset = TAM_REG1_ARV * (rrn + 1);
    } else if (tipo_do_arquivo == 2) {
        byteoffset = TAM_REG2_ARV * (rrn + 1);
    }

    fseek(arq_arvore, byteoffset, SEEK_SET);
}

// Busca por chave com id indicado binariamente no nó de árvore passado
// Tb retorna por referência a posição pos no vetor de chaves em que foi encontrado.
// Se não foi encontrado, a referência retornada será a posição do ponteiro com a qual seguir a busca, se necessário
// Caso a referência para o indice encontrado não interesse, basta passar NULL.
long long int busca_binaria_no_no(int id, int tipo_do_arquivo, no_arvore_t *no_arvore, int *pos) {
    // Busca binária pelo id:
    int posicao_de_retorno = -1;

    int lim_esq = 0;
    int lim_dir = no_arvore->nroChaves - 1;
    
    while (lim_dir - lim_esq > 1) {
        int meio = (lim_esq + lim_dir) / 2;

        if (no_arvore->chaves[meio].id < id) {
            lim_esq = meio + 1;
        } else {
            lim_dir = meio;
        }
    }    

    if (no_arvore->chaves[lim_esq].id == id) {
        if (pos) *pos = lim_esq;
        posicao_de_retorno = (tipo_do_arquivo == 1) ? (long long int)no_arvore->chaves[lim_esq].RRN_dados : no_arvore->chaves[lim_esq].byteoffset_dados;
    }
    else if (no_arvore->chaves[lim_dir].id == id) {
        if (pos) *pos = lim_dir;
        posicao_de_retorno = (tipo_do_arquivo == 1) ? (long long int)no_arvore->chaves[lim_dir].RRN_dados : no_arvore->chaves[lim_dir].byteoffset_dados;
    } else { // Não encontrou, mas ainda temos de retorna com qual ponteiro seguir a busca
        if (pos) {
            if (id < no_arvore->chaves[lim_esq].id) {
                *pos = lim_esq;
            } else if (id < no_arvore->chaves[lim_dir].id) {
                *pos = lim_dir;
            } else {
                *pos = lim_dir + 1;
            }
        }
        posicao_de_retorno = -1;
    }

    return posicao_de_retorno;
}

// Busca por chave com id indicado na árvore toda.
// Retorna -1 se não encontrou ou a referência adequada para o arquivo de dados (RRN ou Byteoffset).
// Retorna por referência o indice:
//      Caso o id tenha sido encontrado, ele representa a posição no vetor de chaves do nó em que ele foi encontrado.
//      Caso o id não tenha sido encontrado, ele represente a posição no vetor de ponteiros do ponteiro que a busca deve seguir.
// Também retorna por referência o ultimo nó analisado, atualizando o campo que indica RRN no arquivo da árvore.
long long int buscar_por_id_na_arvore(int id, int tipo_do_arquivo, FILE *arq_arvore, int *indice, no_arvore_t *ultimo_no) {
    cabecalho_arvore_t cab_arvore = ler_cabecalho_arvore(arq_arvore, tipo_do_arquivo);

    if (cab_arvore.nroNos == 0) return -1; // Árvore vazia -> não encontrou

    int rrn_atual = cab_arvore.noRaiz;

    while (rrn_atual != -1) {
        // Posicionar na raiz e ler nó da árvore

        posicionar_em_rrn_arvore(arq_arvore, tipo_do_arquivo, rrn_atual);
        *ultimo_no = ler_no_arvore(tipo_do_arquivo, arq_arvore);
        ultimo_no->RRN_arvore = rrn_atual;

        long long int posicao_de_retorno = busca_binaria_no_no(id, tipo_do_arquivo, ultimo_no, indice);

        // Se encontrou, retornar a devida posição do arquivo de dados
        if (posicao_de_retorno != -1) return posicao_de_retorno;

        // Se não encontrou, continua busca para descendente adequado
        rrn_atual = ultimo_no->ponteiros[*indice];
    }

    return -1; // Id não foi encontrado na árvore
}

void split(chave_t *nova_chave, int *filho_direito, no_arvore_t *no, chave_t *chave_promo, int *rrn_filho_dir_promo, no_arvore_t *novo_no, int tipo_do_arquivo, cabecalho_arvore_t *cab_arvore) {
    no_arvore_t no_de_trabalho = *no; // copia chaves e ponteiros para nó de trabalho

    // Insere no nó de trabalho na posição adequada:
    // 1 - Abre espaço
    int pos = 0;
    busca_binaria_no_no(nova_chave->id, tipo_do_arquivo, &no_de_trabalho, &pos);
    for (int i = no_de_trabalho.nroChaves - 1; i >= pos; i--) { // Shiftada para esquerda
        no_de_trabalho.chaves[i + 1] = no_de_trabalho.chaves[i];
        no_de_trabalho.ponteiros[i + 2] = no_de_trabalho.ponteiros[i + 1];
    }
    // 2 - Coloca na devida posição
    no_de_trabalho.chaves[pos] = *nova_chave;
    no_de_trabalho.ponteiros[pos + 1] = *filho_direito;
    no_de_trabalho.nroChaves = 4;

    // Inicializa novo nó à direita:
    cab_arvore->nroNos++;
    *novo_no = criar_no_arvore();
    novo_no->RRN_arvore = (cab_arvore->proxRRN)++;
    if ((no->tipoNo == RAIZ && no->ponteiros[pos] == NIL) || no->tipoNo == FOLHA) { 
        // "RAIZ-FOLHA" ou FOLHA vira FOLHA
        no->tipoNo = FOLHA;
    } else {
        // RAIZ! ou INTERMEDIARIO vira INTERMEDIARIO
        no->tipoNo = INTERMEDIARIO;
    }

    // Nó criado à direita terá mesmo tipo do novo tipo do nó à esquerda
    novo_no->tipoNo = no->tipoNo;

    // Setar chave de promoção como a chave do meio.
    *chave_promo = no_de_trabalho.chaves[2];
    // Setar RRN do filho direito do promovido como o RRN do novo nó à direita
    *rrn_filho_dir_promo = novo_no->RRN_arvore;
    
    // <-- 0 1 | ^^ 2 ^^ | 3 -->

    chave_t chave_nula = {.id = NIL, .RRN_dados = NIL, .byteoffset_dados = NIL};

    no->nroChaves = 2;
    no->chaves[0] = no_de_trabalho.chaves[0];
    no->chaves[1] = no_de_trabalho.chaves[1];
    no->chaves[2] = chave_nula;
    no->ponteiros[0] = no_de_trabalho.ponteiros[0];
    no->ponteiros[1] = no_de_trabalho.ponteiros[1];
    no->ponteiros[2] = no_de_trabalho.ponteiros[2];
    no->ponteiros[3] = NIL;

    novo_no->nroChaves = 1;
    novo_no->chaves[0] = no_de_trabalho.chaves[3];
    novo_no->chaves[1] = chave_nula;
    novo_no->chaves[2] = chave_nula;
    novo_no->ponteiros[0] = no_de_trabalho.ponteiros[3];
    novo_no->ponteiros[1] = no_de_trabalho.ponteiros[4];
    novo_no->ponteiros[2] = NIL;
    novo_no->ponteiros[3] = NIL;
}

int inserir(int rrn_atual, chave_t chave, int *rrn_filho_dir_promo, chave_t *chave_promo, int tipo_do_arquivo, cabecalho_arvore_t *cab_arvore, FILE *arq_arvore) {
    if (rrn_atual == NIL) { // Construção a partir de nós folha
        *chave_promo = chave;
        *rrn_filho_dir_promo = NIL;
        return PROMO;
    }

    posicionar_em_rrn_arvore(arq_arvore, tipo_do_arquivo, rrn_atual);
    no_arvore_t no = ler_no_arvore(tipo_do_arquivo, arq_arvore);
    no.RRN_arvore = rrn_atual;

    int pos = 0;
    long long int posicao_de_retorno = busca_binaria_no_no(chave.id, tipo_do_arquivo, &no, &pos);

    if (posicao_de_retorno != NIL) { // Chave já existente, retornar erro
        return ERRO;
    }
 
    chave_t p_b_key = {};
    int p_b_rrn = NIL;

    int valor_de_retorno = inserir(no.ponteiros[pos], chave, &p_b_rrn, &p_b_key, tipo_do_arquivo, cab_arvore, arq_arvore);

    if (valor_de_retorno == SEM_PROMO || valor_de_retorno == ERRO) {
        return valor_de_retorno;
    }

    if (no.nroChaves < 3) { // Tem espaço no nó -> Inserir ordenado
        for (int i = no.nroChaves - 1; i >= pos; i--) { // Shiftada para esquerda
            no.chaves[i + 1] = no.chaves[i];
            no.ponteiros[i + 2] = no.ponteiros[i + 1];
        }

        no.chaves[pos] = p_b_key;
        no.ponteiros[pos + 1] = p_b_rrn;
        no.nroChaves++;

        posicionar_em_rrn_arvore(arq_arvore, tipo_do_arquivo, rrn_atual);
        escrever_no_arvore(&no, tipo_do_arquivo, arq_arvore);

        return SEM_PROMO;
    } else { // Overflow -> split
        no_arvore_t novo_no;
        split(&p_b_key, &p_b_rrn, &no, chave_promo, rrn_filho_dir_promo, &novo_no, tipo_do_arquivo, cab_arvore);

        posicionar_em_rrn_arvore(arq_arvore, tipo_do_arquivo, rrn_atual);
        escrever_no_arvore(&no, tipo_do_arquivo, arq_arvore);

        posicionar_em_rrn_arvore(arq_arvore, tipo_do_arquivo, *rrn_filho_dir_promo);
        escrever_no_arvore(&novo_no, tipo_do_arquivo, arq_arvore);

        return PROMO;
    }
}

void inserir_chave_em_arvore(chave_t *chave_a_inserir, int tipo_do_arquivo, cabecalho_arvore_t *cab_arvore, FILE *arq_arvore) {
    if (cab_arvore->nroNos == 0) { // Árvore Vazia -> colocar 1 raiz
        // Cria nova raiz
        cab_arvore->nroNos++;
        no_arvore_t nova_raiz = criar_no_arvore();
        nova_raiz.tipoNo = RAIZ;
        nova_raiz.nroChaves = 1;
        nova_raiz.RRN_arvore = cab_arvore->proxRRN++;

        // Colocar primeira chave na raiz
        nova_raiz.chaves[0] = *chave_a_inserir;

        // Atualizar raiz no cabeçalho
        cab_arvore->noRaiz = nova_raiz.RRN_arvore;

        // Escrever raiz
        posicionar_em_rrn_arvore(arq_arvore, tipo_do_arquivo, nova_raiz.RRN_arvore);
        escrever_no_arvore(&nova_raiz, tipo_do_arquivo, arq_arvore);

        return;
    }
    
    int rrn_raiz = cab_arvore->noRaiz;

    int rrn_filho_dir_promo = NIL;
    chave_t chave_promo = {};

    int valor_de_retorno = inserir(cab_arvore->noRaiz, *chave_a_inserir, &rrn_filho_dir_promo, &chave_promo, tipo_do_arquivo, cab_arvore, arq_arvore);

    if (valor_de_retorno == PROMO) {
        // Cria nova raiz
        cab_arvore->nroNos++;
        no_arvore_t nova_raiz = criar_no_arvore();
        nova_raiz.tipoNo = RAIZ;
        nova_raiz.nroChaves = 1;
        nova_raiz.RRN_arvore = cab_arvore->proxRRN++;

        // Colocar chave promovida no novo nó raiz
        nova_raiz.chaves[0] = chave_promo;
    
        // Filho Esquerdo: raiz anterior
        nova_raiz.ponteiros[0] = rrn_raiz;
        // Filho Direito: filho direito da chave promovida
        nova_raiz.ponteiros[1] = rrn_filho_dir_promo;

        // Escrever nova raiz
        rrn_raiz = nova_raiz.RRN_arvore;
        posicionar_em_rrn_arvore(arq_arvore, tipo_do_arquivo, rrn_raiz);
        escrever_no_arvore(&nova_raiz, tipo_do_arquivo, arq_arvore);
    }

    cab_arvore->noRaiz = rrn_raiz;
}
