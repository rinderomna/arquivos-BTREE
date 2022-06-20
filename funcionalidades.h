#include "str.h"
#include "registros.h"

// Gera arquvio binário do tipo indicado a partir de um csv
void funcionalidade1(int tipo_do_arquivo, string_t csv, string_t binario_saida);

// Imprime todos os registros não removidos em sequência de um arquivo binário de tipo passado
void funcionalidade2(int tipo_do_arquivo, string_t binario_entrada);

// Checa que as informações do registro batem com as especificaçẽos em nome_campos e valor_campos
int registro_encontrado(registro_t *reg, string_t *nome_campos, string_t *valor_campos, int n);

// Imprime todos os registro não removidos de um arquivo binário de tipo passado, desde que o
// registro satisfaça n condições para valores campo, condições estas lidas nas próximas n
// linhas da entrada padrão
void funcionalidade3(int tipo_de_arquivo, string_t binario_entrada, int n);

// Imprime o registro de RRN passado de um arquivo binário, desde que ele exista e não esteja removido
void funcionalidade4(int tipo_de_arquivo, string_t binario_entrada, int RRN);

void funcionalidade5(int tipo_do_arquivo, string_t arquivo_de_dados, string_t arquivo_de_indice);

void funcionalidade6(int tipo_do_arquivo, string_t binario_entrada, string_t arquivo_de_indice, int n);

void funcionalidade7(int tipo_do_arquivo, string_t binario_entrada, string_t arquivo_de_indice, int n);

void funcionalidade8(int tipo_do_arquivo, string_t binario_entrada, string_t arquivo_de_indice, int n_atualizacoes);
