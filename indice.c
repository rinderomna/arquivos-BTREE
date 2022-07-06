#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "indice.h"

// struct para registro de indice para ambos os tipos de arquivo
struct registro_de_indice_ {
    int id;
    int RRN;
    long long int byte_offset;
};

// struct para indice em RAM
struct indice_ {
    char status;
    registro_de_indice_t **registros;
    int n_registros;
};

// Aloca e retorna registro de índice inicializado com valores padrão
registro_de_indice_t *criar_registro_indice() {
    registro_de_indice_t *ri = (registro_de_indice_t *)malloc(1 * sizeof(registro_de_indice_t));

    ri->id = -1;
    ri->byte_offset = -1;
    ri-> RRN = -1;

    return ri;
}

// Aloca e retorna índice em RAM inicializado com valores padrão
indice_t *criar_indice() {
    indice_t *indice = (indice_t *)malloc(1 * sizeof(indice_t));

    indice->status = '0'; // Inicialmente potencialmente inconsistente
    indice->registros = NULL;
    indice->n_registros = 0; // Inicialmente sem registros

    return indice;
}

// Desaloca memória relacionada a um índice em RAM
void destruir_indice(indice_t *indice) {
    if (!indice) return;

    for (int i = 0; i < indice->n_registros; i++) {
        destruir_registro_de_indice(indice->registros[i]);
    }

    free(indice->registros);
    free(indice);
}

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

// Adiciona um registro de índice a um índice já criado de forma ordenada
void adicionar_registro_a_indice(indice_t *indice, registro_de_indice_t *ri) {
    if (!indice) return;

    // Incrementa o número de regitros de índice
    indice->n_registros++;

    // Realoca espaço para mais um registro de índice
    indice->registros = (registro_de_indice_t **)realloc(indice->registros, indice->n_registros * sizeof(registro_de_indice_t *));

    indice->registros[indice->n_registros - 1] = ri;

    // Ordenar registros em ordem crescente
    Heapsort(indice);
}

// Remove do índice em RAM por id
void remover_do_indice_por_id(int tipo_do_arquivo, indice_t *indice, int id) {
    int indice_de_remocao = -1;

    busca_no_indice(indice, tipo_do_arquivo, id, &indice_de_remocao);

    if (indice_de_remocao == -1) return;

    // desalocar registro removido
    destruir_registro_de_indice(indice->registros[indice_de_remocao]);

    // "shiftada" para esquerda a partir da posição removida
    for(int i = indice_de_remocao; i < indice->n_registros - 1; i++) {
        indice->registros[i] = indice->registros[i + 1];
    } 

    indice->n_registros--;
}

// Desaloca memória relacionada ao registro de índice
void destruir_registro_de_indice(registro_de_indice_t *ri) {
    if (ri) free(ri);
}

// Escreve registro de tipo indicado no arquivo de índice já aberto com permissão para escrita
void escrever_registro_no_indice(registro_de_indice_t *ri, int tipo_do_arquivo, FILE *arquivo_de_indice) {
    // Escreve o id no primeiro campo
    fwrite(&ri->id, sizeof(int), 1, arquivo_de_indice);
    
    // Escreve RRN ou Byte Offset no segundo campo
    if (tipo_do_arquivo == 1) {
        fwrite(&ri->RRN, sizeof(int), 1, arquivo_de_indice);
    } else if (tipo_do_arquivo == 2) {
        fwrite(&ri->byte_offset, sizeof(long long int), 1, arquivo_de_indice);
    }
}

// Lê registro de tipo indicado do arquivo de índice já aberto com permissão para leitura
// Retorna se algo foi lido ou não
int ler_registro_do_indice(registro_de_indice_t *ri, int tipo_do_arquivo, FILE *arquivo_de_indice) {
    // Lê o id do primeiro campo
    int lido = fread(&ri->id, sizeof(int), 1, arquivo_de_indice);

    if (!lido) return 0; // Nada foi lido

    // Lê RRN ou Byte Offset no segundo campo
    if (tipo_do_arquivo == 1) {
        fread(&ri->RRN, sizeof(int), 1, arquivo_de_indice);
    } else if (tipo_do_arquivo == 2) {
        fread(&ri->byte_offset, sizeof(long long int), 1, arquivo_de_indice);
    }

    return 1; // Foi lido algo
}

// Lê status do cabeçalho de um arquivo de índice já aberto com permissão para leitura
char ler_status_do_indice(FILE *arquivo_de_indice) {
    char status = '0';

    // Posiciona no início do arquivo
    fseek(arquivo_de_indice, 0, SEEK_SET);

    // Lê o status do índice
    fread(&status, sizeof(char), 1, arquivo_de_indice);

    return status;    
}

// Escreve status no cabeçalho de um arquivo de índice já aberto com permissão para escrita
void escrever_status_no_indice(char status, FILE *arquivo_de_indice) {
    // Posiciona no início do arquivo
    fseek(arquivo_de_indice, 0, SEEK_SET);

    // Escreve status passado no arquivo de índice
    fwrite(&status, sizeof(char), 1, arquivo_de_indice);    
}

// Carrega índice para a RAM
indice_t *ler_indice(int tipo_do_arquivo, string_t nome_do_arquivo_de_indice) {
    FILE *arq_indice = NULL;
    arq_indice = fopen(nome_do_arquivo_de_indice, "rb");

    if (!arq_indice) {
        printf("Falha no processamento do arquivo.\n");

        return NULL;
    }

    char status = ler_status_do_indice(arq_indice);

    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(arq_indice);

        return NULL;
    }

    indice_t *indice = criar_indice();
    indice->status = status;

    int lido = 1;

    do {
        registro_de_indice_t *ri = criar_registro_indice();
        
        lido = ler_registro_do_indice(ri, tipo_do_arquivo, arq_indice);

        if (lido) {
            adicionar_registro_a_indice(indice, ri);
        } else {
            destruir_registro_de_indice(ri);
        }
    } while (lido);

    fclose(arq_indice);

    return indice;
}

// Escreve índice em RAM em arquivo de índice
void escrever_indice(indice_t *indice, int tipo_de_arquivo, string_t nome_do_arquivo_de_indice) {
    FILE *arq_indice = fopen(nome_do_arquivo_de_indice, "wb");

    // Inicialmente potencialmente inconsistente
    escrever_status_no_indice('0', arq_indice);

    // Escreve todos os registro no arquivo de índice em RAM
    for (int i = 0; i < indice->n_registros; i++) {
        escrever_registro_no_indice(indice->registros[i], tipo_de_arquivo, arq_indice);
    }

    // Finaliza consistente
    set_status_indice(indice, '1');
    escrever_status_no_indice('1', arq_indice);

    // Fechar arquivo
    fclose(arq_indice);
}

// Retorna RRN ou Byteoffset do registro de id buscado, ou -1 caso não encontrado.
long long int busca_no_indice(indice_t *indice, int tipo_do_arquivo, int id, int *indice_encontrado) {
    // Busca binária pelo id:
    int posicao_de_retorno = -1;

    int lim_esq = 0;
    int lim_dir = indice->n_registros - 1;
    
    while (lim_dir - lim_esq > 1) {
        int meio = (lim_esq + lim_dir) / 2;

        if (get_id_registro_indice(indice->registros[meio]) < id) {
            lim_esq = meio + 1;
        } else {
            lim_dir = meio;
        }
    }    

    if (get_id_registro_indice(indice->registros[lim_esq]) == id) {
        if (indice_encontrado) *indice_encontrado = lim_esq;
        posicao_de_retorno = (tipo_do_arquivo == 1) ? (long long int)get_rrn_registro_indice(indice->registros[lim_esq]) : get_byte_offset_registro_indice(indice->registros[lim_esq]);
    }
    else if (get_id_registro_indice(indice->registros[lim_dir]) == id) {
        if (indice_encontrado) *indice_encontrado = lim_dir;
        posicao_de_retorno = (tipo_do_arquivo == 1) ? (long long int)get_rrn_registro_indice(indice->registros[lim_dir]) : get_byte_offset_registro_indice(indice->registros[lim_dir]);
    } else {
        if (indice_encontrado) *indice_encontrado = -1;
        posicao_de_retorno = -1;
    }
    
    return posicao_de_retorno;
}

// Getters & Setters:

void set_status_indice(indice_t *indice, char status) {
    indice->status = status;
}

char get_status_indice(indice_t *indice) {
    return indice->status;
}

int get_n_registros(indice_t *indice) {
    return indice->n_registros;
}

registro_de_indice_t **get_registros(indice_t *indice) {
    return indice->registros;
}

void set_id_registro_indice(registro_de_indice_t *ri, int id) {
    ri->id = id;
}

int get_id_registro_indice(registro_de_indice_t *ri) {
    return ri->id;
}

void set_rrn_registro_indice(registro_de_indice_t *ri, int rrn) {
    ri->RRN = rrn;
}

int get_rrn_registro_indice(registro_de_indice_t *ri) {
    return ri->RRN;
}

void set_byte_offset_registro_indice(registro_de_indice_t *ri, long long int byte_offset) {
    ri->byte_offset = byte_offset;
}

long long int get_byte_offset_registro_indice(registro_de_indice_t *ri) {
    return ri->byte_offset;
}

void imprime_registro_indice(registro_de_indice_t *ri, int tipo_do_arquivo) {
    printf("%d - ", ri->id);
    
    if (tipo_do_arquivo == 1) {
        printf("%d\n", ri->RRN);
    } else if (tipo_do_arquivo == 2) {
        printf("%lld\n", ri->byte_offset);
    }
}

void imprime_indice(indice_t *indice, int tipo_do_arquivo) {
    printf("status: %c\n\n", indice->status);

    for (int i = 0; i < indice->n_registros; i++) {
        imprime_registro_indice(indice->registros[i], tipo_do_arquivo);
    }
}
