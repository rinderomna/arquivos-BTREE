/* |Trabalho 2 de Arquivos - 2022/01|
 * Hélio Nogueira Cardoso         - N°USP: 10310227
 * Raphael David Philippe Leveque - N°USP: 12542522
 */

#include <stdio.h>
#include <stdlib.h>
#include "funcionalidades.h"
#include "str.h"
#include "indice.h"

int main() {
    // Ler a linha de comando toda:
    int has_EOF = 0;
    string_t linha_de_comando = read_line(stdin, &has_EOF);

    // Separar a linha de comando nos espaços:
    int n_campos = 0;
    string_t *campos_de_comando = split_string(linha_de_comando, ' ', &n_campos);
    destroy_string(linha_de_comando);

    // Primeiro campo é o código da funcionalidade:
    int codigo_de_funcionalidade = atoi(campos_de_comando[0]);

    string_t tipo_do_arquivo;
    string_t csv;
    string_t binario_saida;
    string_t binario_entrada;
    string_t arquivo_de_indice;
    int RRN;
    int n;

    tipo_do_arquivo = campos_de_comando[1];
    int tipo = 0;
    if (compare_strings_case_sensitive(tipo_do_arquivo, "tipo1") == 0) {
        tipo = 1;
    } else if (compare_strings_case_sensitive(tipo_do_arquivo, "tipo2") == 0) {
        tipo = 2;
    }

    switch (codigo_de_funcionalidade) {
        case 1:
            csv = campos_de_comando[2];
            binario_saida = campos_de_comando[3];

            funcionalidade1(tipo, csv, binario_saida);
            break;
        case 2:
            binario_entrada = campos_de_comando[2];

            funcionalidade2(tipo, binario_entrada);
            break;
        case 3:
            binario_entrada = campos_de_comando[2];
            n = atoi(campos_de_comando[3]);
            funcionalidade3(tipo, binario_entrada, n);
            break;
        case 4:
            binario_entrada = campos_de_comando[2];
            RRN = atoi(campos_de_comando[3]);

            funcionalidade4(tipo, binario_entrada, RRN);
            break;
        case 5:
            binario_entrada = campos_de_comando[2];
            binario_saida = campos_de_comando[3];

            funcionalidade5(tipo, binario_entrada, binario_saida);
            break;
        case 6:
            binario_entrada = campos_de_comando[2];
            arquivo_de_indice = campos_de_comando[3];
            n = atoi(campos_de_comando[4]);

            funcionalidade6(tipo, binario_entrada, arquivo_de_indice, n);
            break;
        case 7:
            binario_entrada = campos_de_comando[2];
            arquivo_de_indice = campos_de_comando[3];
            n = atoi(campos_de_comando[4]);

            funcionalidade7(tipo, binario_entrada, arquivo_de_indice, n);
        default:
            break;
    }

    destroy_string_array(campos_de_comando, n_campos);

    return 0;
}