#include <stdio.h>
#include "str.h"

typedef struct registro_de_indice_ registro_de_indice_t;
typedef struct indice_ indice_t;

// Aloca e retorna registro de índice inicializado com valores padrão
registro_de_indice_t *criar_registro_indice();

// Desaloca memória relacionada ao registro de índice
void destruir_registro_de_indice(registro_de_indice_t *ri);

// Aloca e retorna índice em RAM inicializado com valores padrão
indice_t *criar_indice();

// Desaloca memória relacionada a um índice em RAM
void destruir_indice(indice_t *indice);

// Adiciona um registro de índice a um índice já criado de forma ordenada
void adicionar_registro_a_indice(indice_t *indice, registro_de_indice_t *ri);

// Carrega índice de arquivo para RAM alocando e retornando
indice_t *ler_indice(int tipo_do_arquivo, string_t nome_do_arquivo_de_indice);

// Lê status do cabeçalho de um arquivo de índice já aberto com permissão para leitura
char ler_status_do_indice(FILE *arquivo_de_indice);

// Escreve status no cabeçalho de um arquivo de índice já aberto com permissão para escrita
void escrever_status_no_indice(char status, FILE *arquivo_de_indice);

// Escreve índice em RAM em arquivo de índice
void escrever_indice(indice_t *indice, int tipo_de_arquivo, string_t nome_do_arquivo_de_indice);

// Busca de forma binária por id no indice indicado
long long int busca_no_indice(string_t nome_arquivo_de_indice, int tipo_do_arquivo, int id);

// Getters & Setters:

int get_n_registros(indice_t *indice);
void set_status_indice(indice_t *indice, char status);
char get_status_indice(indice_t *indice);
registro_de_indice_t **get_registros(indice_t *indice);
void set_id_registro_indice(registro_de_indice_t *ri, int id);
int get_id_registro_indice(registro_de_indice_t *ri);
void set_rrn_registro_indice(registro_de_indice_t *ri, int rrn);
int get_rrn_registro_indice(registro_de_indice_t *ri);
void set_byte_offset_registro_indice(registro_de_indice_t *ri, long long int byte_offset);
long long int get_byte_offset_registro_indice(registro_de_indice_t *ri);

//
void imprime_indice(indice_t *indice, int tipo_do_arquivo);