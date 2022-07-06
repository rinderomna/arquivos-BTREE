#include "str.h"
#include "registros.h"

// Gera arquvio binário do tipo indicado a partir de um csv
void funcionalidade1(int tipo_do_arquivo, string_t csv, string_t binario_saida);

// Imprime todos os registros não removidos em sequência de um arquivo binário de tipo passado
void funcionalidade2(int tipo_do_arquivo, string_t binario_entrada);

// Testa se um registro bate com as especificações em nome_campos e valor_campos, retornando um inteiro booleano
int registro_encontrado(registro_t *reg, string_t *nome_campos, string_t *valor_campos, int n);

// Imprime todos os registro não removidos de um arquivo binário de tipo passado, desde que o registro 
// satisfaça 'n' condições para valores campo, condições estas lidas nas próximas n linhas da entrada padrão
void funcionalidade3(int tipo_de_arquivo, string_t binario_entrada, int n);

// Imprime o registro de RRN passado de um arquivo binário, desde que ele exista e não esteja removido
void funcionalidade4(int tipo_de_arquivo, string_t binario_entrada, int RRN);

// Cria um arquivo de índice primário a partir de um arquivo de dados existente de tipo indicado
// A chave primária é o campo id do registros de dados
// Para o tipo1, guarda-se para cada chave primária o RRN do registro no arquivo de dados
// Para o tipo2, guarda-se para cada chave primária o Byteoffset do registro no arquivo de dados
void funcionalidade5(int tipo_do_arquivo, string_t arquivo_de_dados, string_t arquivo_de_indice);

// Remove do arquivo de dados e de índice correspondente 'n_remocoes' registros especificados na entrada padrão
void funcionalidade6(int tipo_do_arquivo, string_t binario_entrada, string_t arquivo_de_indice, int n_remocoes);

// Insere no arquivo de dados e de índice correspondente 'n_insercoes' novos registro lidos entrada padrão
void funcionalidade7(int tipo_do_arquivo, string_t binario_entrada, string_t arquivo_de_indice, int n_insercoes);

// Altera no arquivo de dados e de índice correspondente 'n_alteracoes' registro lidos especificados na entrada padrão
void funcionalidade8(int tipo_do_arquivo, string_t binario_entrada, string_t arquivo_de_indice, int n_alteracoes);

// Cria um arquivo de índice primário árvore B a partir de um arquivo de dados existente de tipo indicado
// A chave primária é o campo id do registros de dados
// Para o tipo1, guarda-se para cada chave primária o RRN do registro no arquivo de dados
// Para o tipo2, guarda-se para cada chave primária o Byteoffset do registro no arquivo de dados
void funcionalidade9(int tipo_do_arquivo, string_t arquivo_de_dados, string_t arquivo_de_indice);

// Insere no arquivo de dados e de índice árvore-B correspondente 'n' novos registro lidos entrada padrão
void funcionalidade11(int tipo_do_arquivo, string_t binario_entrada, string_t arquivo_de_indice, int n_insercoes);
