#include <stdio.h>
#include <stdlib.h>
#include "funcionalidades.h"
#include "str.h"

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
    int RRN;
    int n;

    switch (codigo_de_funcionalidade) {
        case 1:
            tipo_do_arquivo = campos_de_comando[1];
            csv = campos_de_comando[2];
            binario_saida = campos_de_comando[3];

            funcionalidade1(tipo_do_arquivo, csv, binario_saida);
            break;
        case 2:
            tipo_do_arquivo = campos_de_comando[1];
            binario_entrada = campos_de_comando[2];

            funcionalidade2(tipo_do_arquivo, binario_entrada);
            break;
        case 3:
            tipo_do_arquivo = campos_de_comando[1];
            binario_entrada = campos_de_comando[2];
            n = atoi(campos_de_comando[3]);
            funcionalidade3(tipo_do_arquivo, binario_entrada, n);
            break;
        case 4:
            tipo_do_arquivo = campos_de_comando[1];
            binario_entrada = campos_de_comando[2];
            RRN = atoi(campos_de_comando[3]);

            funcionalidade4(tipo_do_arquivo, binario_entrada, RRN);
            break;
        default:
            break;
    }

    destroy_string_array(campos_de_comando, n_campos);

    return 0;
}