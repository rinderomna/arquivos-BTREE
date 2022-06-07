#include <stdio.h>
#include <stdlib.h>

#include "cabecalhos.h"
#include "str.h"


// Cabeçalho para Arquivo com Registros de Tamanho Fixo
struct cabecalho_ {
    char status;
    long long int topo;
    char descricao[40+1];
    char desC1[22+1];
    char desC2[19+1];
    char desC3[24+1];
    char desC4[8+1];
    char codC5; // Código de Cidade = 0
    char desC5[16+1];
    char codC6; // Código de Marca = 1
    char desC6[18+1];
    char codC7; // Código de Modelo = 2
    char desC7[19+1];
    int proxRRN;
    long long int proxByteOffset;
    int nroRegRem;
};

cabecalho_t *criar_cabecalho() {
    cabecalho_t *cabecalho = (cabecalho_t *)malloc(1 * sizeof(cabecalho_t));
    cabecalho->status = '0'; // Inicialmente potencialmente inconsistente
    cabecalho->topo = -1; // Inicialmente sem registros removidos
    copy_memory(cabecalho->descricao,"LISTAGEM DA FROTA DOS VEICULOS NO BRASIL", 41);
    copy_memory(cabecalho->desC1,"CODIGO IDENTIFICADOR: ", 23);
    copy_memory(cabecalho->desC2,"ANO DE FABRICACAO: ", 20);
    copy_memory(cabecalho->desC3,"QUANTIDADE DE VEICULOS: ", 25);
    copy_memory(cabecalho->desC4,"ESTADO: ", 9);
    cabecalho->codC5 = '0';
    copy_memory(cabecalho->desC5,"NOME DA CIDADE: ", 17);
    cabecalho->codC6 = '1';
    copy_memory(cabecalho->desC6,"MARCA DO VEICULO: ", 19);
    cabecalho->codC7 = '2';
    copy_memory(cabecalho->desC7,"MODELO DO VEICULO: ", 20);
    cabecalho->proxRRN = 0;
    cabecalho->proxByteOffset = 0;
    cabecalho->nroRegRem = 0;

    return cabecalho;
}

// Escreve um cabeçalho do tipo indicado no início de um arquivo já aberto com permissão para escrita
void escrever_cabecalho_em_arquivo(cabecalho_t *cabecalho, int tipo_do_arquivo, FILE *fp) {
    fseek(fp, 0, SEEK_SET);
    fwrite(&cabecalho->status, sizeof(char), 1, fp);
    if (tipo_do_arquivo == 1) {
        fwrite(&cabecalho->topo, sizeof(int), 1, fp);
    } else if (tipo_do_arquivo == 2) {
        fwrite(&cabecalho->topo, sizeof(long long int), 1, fp);
    }
    fwrite(cabecalho->descricao, sizeof(char), 40, fp);
    fwrite(cabecalho->desC1, sizeof(char), 22, fp);
    fwrite(cabecalho->desC2, sizeof(char), 19, fp);
    fwrite(cabecalho->desC3, sizeof(char), 24, fp);
    fwrite(cabecalho->desC4, sizeof(char), 8, fp);
    fwrite(&cabecalho->codC5, sizeof(char), 1, fp);
    fwrite(cabecalho->desC5, sizeof(char), 16, fp);
    fwrite(&cabecalho->codC6, sizeof(char), 1, fp);
    fwrite(cabecalho->desC6, sizeof(char), 18, fp);
    fwrite(&cabecalho->codC7, sizeof(char), 1, fp);
    fwrite(cabecalho->desC7, sizeof(char), 19, fp);
    if (tipo_do_arquivo == 1) {
        fwrite(&cabecalho->proxRRN, sizeof(int), 1, fp);
    } else if (tipo_do_arquivo == 2) {
        fwrite(&cabecalho->proxByteOffset, sizeof(long long int), 1, fp);
    }
    fwrite(&cabecalho->nroRegRem, sizeof(int), 1, fp);
}

// Lê cabeçalho tipo indicado de arquivo já aberto com permissão para leitura
void ler_cabecalho_de_arquivo(cabecalho_t *cabecalho, int tipo_do_arquivo, FILE *fp) {
    fseek(fp, 0, SEEK_SET);
    fread(&cabecalho->status, sizeof(char), 1,fp);
    if (tipo_do_arquivo == 1) {
        fread(&cabecalho->topo, sizeof(int), 1, fp);
        fseek(fp,169,SEEK_CUR);
        fread(&cabecalho->proxRRN, sizeof(int), 1, fp);
    } else if (tipo_do_arquivo == 2) {
        fread(&cabecalho->topo, sizeof(long long int), 1, fp);
        fseek(fp,169,SEEK_CUR);
        fread(&cabecalho->proxByteOffset, sizeof(long long int), 1, fp);
    }
    fread(&cabecalho->nroRegRem, sizeof(int), 1, fp);
}

void destruir_cabecalho(cabecalho_t *cabecalho) {
    if (cabecalho) {
        free(cabecalho);
    }
}

// Getters e Setters:

void set_status(cabecalho_t *cabecalho, char status) {
    cabecalho->status = status;
}
char get_status(cabecalho_t *cabecalho) {
    return cabecalho->status;
}

void set_topo(cabecalho_t *cabecalho, long long int topo) {
    cabecalho->topo = topo;
}

long long int get_topo(cabecalho_t *cabecalho) {
    return cabecalho->topo;
}

void set_proxRRN(cabecalho_t *cabecalho, int proxRRN) {
    cabecalho->proxRRN = proxRRN;
}

int get_proxRRN(cabecalho_t *cabecalho) {
    return cabecalho->proxRRN;
}

void set_nroRegRem(cabecalho_t *cabecalho, int nroRegRem) {
    cabecalho->nroRegRem = nroRegRem;
}

int get_nroRegRem(cabecalho_t *cabecalho) {
    return cabecalho->nroRegRem;
}

void set_proxByteOffset(cabecalho_t *cabecalho, long long int proxByteOffset) {
    cabecalho->proxByteOffset = proxByteOffset;
}

long long int get_proxByteOffset(cabecalho_t *cabecalho) {
    return cabecalho->proxByteOffset;
}