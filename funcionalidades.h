#include "str.h"

// Gera arquvio binário do tipo indicado a partir de um csv
void funcionalidade1(int tipo_do_arquivo, string_t csv, string_t binario_saida);

// Imprime todos os registros não removidos em sequência de um arquivo binário de tipo passado
void funcionalidade2(int tipo_do_arquivo, string_t binario_entrada);

// Imprime todos os registro não removidos de um arquivo binário de tipo passado, desde que o
// registro satisfaça n condições para valores campo, condições estas lidas nas próximas n
// linhas da entrada padrão
void funcionalidade3(int tipo_de_arquivo, string_t binario_entrada, int n);

// Imprime o registro de RRN passado de um arquivo binário, desde que ele exista e não esteja removido
void funcionalidade4(int tipo_de_arquivo, string_t binario_entrada, int RRN);