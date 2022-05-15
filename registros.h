#include "str.h"

#define TAM_REG1 97

typedef struct registro1_ registro1_t;
typedef struct registro2_ registro2_t;

/* Funcões para registro tipo 1 */

// Aloca e retorna registro tipo 1 inicializado com valores padrão
registro1_t *criar_registro1();

// Escreve registro tipo 1 em arquivo já aberto para escrita
void escrever_registro1_em_arquivo(registro1_t *reg1, FILE *fp);

// Lê 97 bytes preenchendo registro de tamanho fixo
void ler_registro1(registro1_t *reg1, FILE *fp);

// Imprime informações de um registro em formato padrão
void imprimir_reg1(registro1_t *reg1);

// Libera a memória relacionada a registro tipo 1.
// Flag booleana para liberar strings ou não
void destruir_registro1(registro1_t *reg1, int liberar_strings);

// Getters e Setters:
void set_removido1(registro1_t *reg1, char removido);
char get_removido1(registro1_t *reg1);
void set_prox1(registro1_t *reg1, int prox);
int get_prox1(registro1_t *reg1);
void set_id1(registro1_t *reg1, int id);
int get_id1(registro1_t *reg1);
void set_ano1(registro1_t *reg1, int ano);
int get_ano1(registro1_t *reg1);
void set_qtt1(registro1_t *reg1, int qtt);
int get_qtt1(registro1_t *reg1);
void set_sigla1(registro1_t *reg1, string_t sigla);
string_t get_sigla1(registro1_t *reg1);
void set_cidade1(registro1_t *reg1, string_t cidade);
string_t get_cidade1(registro1_t *reg1);
void set_marca1(registro1_t *reg1, string_t marca);
string_t get_marca1(registro1_t *reg1);
void set_modelo1(registro1_t *reg1, string_t modelo);
string_t get_modelo1(registro1_t *reg1);

/* Funcões para registro tipo 2 */

// Aloca e retorna registro tipo 2 inicializado com valores padrão
registro2_t *criar_registro2();

// Escreve registro tipo 2 em arquivo já aberto para escrita
int escrever_registro2_em_arquivo(registro2_t *reg2, FILE *fp);

// Lê registro de tamanho fixo armazenado, retornando tamanho total do registro lido
int ler_registro2(registro2_t *reg2, FILE *fp);

// Imprime informações de um registro em formato padrão
void imprimir_reg2(registro2_t *reg2);

// Libera a memória relacionada a registro tipo 2
// Flag booleana para liberar strings ou não
void destruir_registro2(registro2_t *reg2, int liberar_strings);

// Getters e Setters:
void set_removido2(registro2_t *reg2, char removido);
char get_removido2(registro2_t *reg2);
void set_prox2(registro2_t *reg2, long long int prox);
long long int get_prox2(registro2_t *reg2);
void set_id2(registro2_t *reg2, int id);
int get_id2(registro2_t *reg2);
void set_ano2(registro2_t *reg2, int ano);
int get_ano2(registro2_t *reg2);
void set_qtt2(registro2_t *reg2, int qtt);
int get_qtt2(registro2_t *reg2);
void set_sigla2(registro2_t *reg2, string_t sigla);
string_t get_sigla2(registro2_t *reg2);
void set_cidade2(registro2_t *reg2, string_t cidade);
string_t get_cidade2(registro2_t *reg2);
void set_marca2(registro2_t *reg2, string_t marca);
string_t get_marca2(registro2_t *reg2);
void set_modelo2(registro2_t *reg2, string_t modelo);
string_t get_modelo2(registro2_t *reg2);