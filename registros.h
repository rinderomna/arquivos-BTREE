#include "str.h"

#define TAM_REG1 97

typedef struct registro_ registro_t;

// Aloca e retorna registro inicializado com valores padrão
registro_t *criar_registro();

// Libera a memória relacionada ao registro
// Flag booleana para liberar strings ou não
void destruir_registro(registro_t *reg, int liberar_strings);

// Escreve registro tipo 1 em arquivo já aberto para escrita
void escrever_registro1_em_arquivo(registro_t *reg1, FILE *fp);

// Escreve registro tipo 2 em arquivo já aberto para escrita
int escrever_registro2_em_arquivo(registro_t *reg2, FILE *fp);

// Escrever registro tipo 2 em cima de registro já existente
void reescrever_registro2_em_arquivo(registro_t *reg2, int tam_registro_anterior, FILE *fp);

// Lê registro de tamanho fixo armazenado, retornando tamanho total do registro lido
// Precisa liberar string ao destruir registro lido
int ler_registro(registro_t *reg, int tipo_do_registro, FILE *fp);

// Imprime informações de um registro em formato padrão
void imprimir_registro(registro_t *reg);

// Calcula o tamanho total de um registro baseado em seu tipo
int tamanho_total_do_registro(int tipo_do_arquivo, registro_t *reg);

// Busca o registro no arquivo na posição indicada, que pode ser rrn (tipo 1) ou byte offset (tipo 2)
registro_t *get_registro_em_posicao(int tipo_do_arquivo, long long int posicao, FILE *fp);

// Escreve o registro no arquivo na posição indicada, que pode ser rrn (tipo 1) ou byte offset (tipo 2)
void escrever_registro_em_posicao(int tipo_do_arquivo, registro_t *reg, long long int posicao, FILE *fp);

// Getters e Setters:
void set_removido(registro_t *reg, char removido);
char get_removido(registro_t *reg);
void set_proxRRN_removido(registro_t *reg, int proxRRN);
int get_proxRRN_removido(registro_t *reg);
void set_proxByteOffset_removido(registro_t *reg, long long int proxByteOffset);
long long int get_proxByteOffset_removido(registro_t *reg);
void set_id(registro_t *reg, int id);
int get_id(registro_t *reg);
void set_ano(registro_t *reg, int ano);
int get_ano(registro_t *reg);
void set_qtt(registro_t *reg, int qtt);
int get_qtt(registro_t *reg);
void set_sigla(registro_t *reg, string_t sigla);
string_t get_sigla(registro_t *reg);
void set_cidade(registro_t *reg, string_t cidade);
string_t get_cidade(registro_t *reg);
void set_marca(registro_t *reg, string_t marca);
string_t get_marca(registro_t *reg);
void set_modelo(registro_t *reg, string_t modelo);
string_t get_modelo(registro_t *reg);