#include <stdio.h>
#include <stdlib.h>

#include "cabecalhos.h"
#include "str.h"

// Cabeçalho para Arquivo com Registros de Tamanho Fixo
struct cabecalho1_ {
    char status;
    int topo;
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
    int nroRegRem;
};

// Cabeçalho para Arquivo com Registros de Tamanho Fixo
struct cabecalho2_ {
    char status;
    long long int topo;
    char descricao[40+1];
    char desC1[22+1];
    char desC2[19+1];
    char desC3[24+1];
    char desC4[8+1];
    char codC5;
    char desC5[16+1];
    char codC6;
    char desC6[18+1];
    char codC7;
    char desC7[19+1];
    long long int proxByteOffset;
    int nroRegRem;
};

cabecalho1_t *criar_cabecalho1() {
    cabecalho1_t *c1 = (cabecalho1_t *)malloc(1 * sizeof(cabecalho1_t));
    c1->status = '0'; // Inicialmente potencialmente inconsistente
    c1->topo = -1; // Inicialmente sem registros removidos
    copy_memory(c1->descricao,"LISTAGEM DA FROTA DOS VEICULOS NO BRASIL", 41);
    copy_memory(c1->desC1,"CODIGO IDENTIFICADOR: ", 23);
    copy_memory(c1->desC2,"ANO DE FABRICACAO: ", 20);
    copy_memory(c1->desC3,"QUANTIDADE DE VEICULOS: ", 25);
    copy_memory(c1->desC4,"ESTADO: ", 9);
    c1->codC5 = '0';
    copy_memory(c1->desC5,"NOME DA CIDADE: ", 17);
    c1->codC6 = '1';
    copy_memory(c1->desC6,"MARCA DO VEICULO: ", 19);
    c1->codC7 = '2';
    copy_memory(c1->desC7,"MODELO DO VEICULO: ", 20);
    c1->proxRRN = 0;
    c1->nroRegRem = 0;

    return c1;
}

// Getters e Setters:

void set_status1(cabecalho1_t *c1, char status) {
    c1->status = status;
}
char get_status1(cabecalho1_t *c1) {
    return c1->status;
}

void set_topo1(cabecalho1_t *c1, int topo) {
    c1->topo = topo;
}

int get_topo1(cabecalho1_t *c1) {
    return c1->topo;
}

void set_proxRRN1(cabecalho1_t *c1, int proxRRN) {
    c1->proxRRN = proxRRN;
}

int get_proxRRN1(cabecalho1_t *c1) {
    return c1->proxRRN;
}

void set_nroRegRem1(cabecalho1_t *c1, int nroRegRem) {
    c1->nroRegRem = nroRegRem;
}

int get_nroRegRem1(cabecalho1_t *c1) {
    return c1->nroRegRem;
}

// Escreve um cabeçalho tipo 1 no início de um arquivo já aberto com permissão para escrita
void escrever_cabecalho1_em_arquivo(cabecalho1_t *c1, FILE *fp) {
    fseek(fp, 0, SEEK_SET);
    fwrite(&c1->status, sizeof(char), 1, fp);
    fwrite(&c1->topo, sizeof(int), 1, fp);
    fwrite(c1->descricao, sizeof(char), 40, fp);
    fwrite(c1->desC1, sizeof(char), 22, fp);
    fwrite(c1->desC2, sizeof(char), 19, fp);
    fwrite(c1->desC3, sizeof(char), 24, fp);
    fwrite(c1->desC4, sizeof(char), 8, fp);
    fwrite(&c1->codC5, sizeof(char), 1, fp);
    fwrite(c1->desC5, sizeof(char), 16, fp);
    fwrite(&c1->codC6, sizeof(char), 1, fp);
    fwrite(c1->desC6, sizeof(char), 18, fp);
    fwrite(&c1->codC7, sizeof(char), 1, fp);
    fwrite(c1->desC7, sizeof(char), 19, fp);
    fwrite(&c1->proxRRN, sizeof(int), 1, fp);
    fwrite(&c1->nroRegRem, sizeof(int), 1, fp);
}

// Lê cabeçalho tipo 1 de arquivo já aberto com permissão para leitura
void ler_cabecalho1_de_arquivo(cabecalho1_t *c1, FILE *fp) {
    fseek(fp, 0, SEEK_SET);
    fread(&c1->status, sizeof(char), 1,fp);
    fread(&c1->topo, sizeof(int), 1, fp);
    fseek(fp,169,SEEK_CUR);
    fread(&c1->proxRRN, sizeof(int), 1, fp);
    fread(&c1->nroRegRem, sizeof(int), 1, fp);
}

void destruir_cabecalho1(cabecalho1_t *c1) {
    free(c1);
}

cabecalho2_t *criar_cabecalho2() {
    cabecalho2_t *c2 = (cabecalho2_t *)malloc(1 * sizeof(cabecalho2_t));
    c2->status = '0'; // Inicialmente potencialmente inconsistente
    c2->topo = -1; // Inicialmente sem registros removidos
    copy_memory(c2->descricao,"LISTAGEM DA FROTA DOS VEICULOS NO BRASIL", 41);
    copy_memory(c2->desC1,"CODIGO IDENTIFICADOR: ", 23);
    copy_memory(c2->desC2,"ANO DE FABRICACAO: ", 20);
    copy_memory(c2->desC3,"QUANTIDADE DE VEICULOS: ", 25);
    copy_memory(c2->desC4,"ESTADO: ", 9);
    c2->codC5 = '0';
    copy_memory(c2->desC5,"NOME DA CIDADE: ", 17);
    c2->codC6 = '1';
    copy_memory(c2->desC6,"MARCA DO VEICULO: ", 19);
    c2->codC7 = '2';
    copy_memory(c2->desC7,"MODELO DO VEICULO: ", 20);
    c2->proxByteOffset = 0;
    c2->nroRegRem = 0;

    return c2;
}

// Getters e Setters:

void set_status2(cabecalho2_t *c2, char status) {
    c2->status = status;
}
char get_status2(cabecalho2_t *c2) {
    return c2->status;
}

void set_topo2(cabecalho2_t *c2, long long int topo) {
    c2->topo = topo;
}

long long int get_topo2(cabecalho2_t *c2) {
    return c2->topo;
}

void set_proxByteOffset2(cabecalho2_t *c2, long long int proxByteOffset) {
    c2->proxByteOffset = proxByteOffset;
}

long long int get_proxByteOffset2(cabecalho2_t *c2) {
    return c2->proxByteOffset;
}

void set_nroRegRem2(cabecalho2_t *c2, int nroRegRem) {
    c2->nroRegRem = nroRegRem;
}

int get_nroRegRem2(cabecalho2_t *c2) {
    return c2->nroRegRem;
}

// Escreve um cabeçalho tipo 2 no início de um arquivo já aberto com permissão para escrita
void escrever_cabecalho2_em_arquivo(cabecalho2_t *c2, FILE *fp) {
    fseek(fp, 0, SEEK_SET);
    fwrite(&c2->status, sizeof(char), 1, fp);
    fwrite(&c2->topo, sizeof(long long int), 1, fp);
    fwrite(c2->descricao, sizeof(char), 40, fp);
    fwrite(c2->desC1, sizeof(char), 22, fp);
    fwrite(c2->desC2, sizeof(char), 19, fp);
    fwrite(c2->desC3, sizeof(char), 24, fp);
    fwrite(c2->desC4, sizeof(char), 8, fp);
    fwrite(&c2->codC5, sizeof(char), 1, fp);
    fwrite(c2->desC5, sizeof(char), 16, fp);
    fwrite(&c2->codC6, sizeof(char), 1, fp);
    fwrite(c2->desC6, sizeof(char), 18, fp);
    fwrite(&c2->codC7, sizeof(char), 1, fp);
    fwrite(c2->desC7, sizeof(char), 19, fp);
    fwrite(&c2->proxByteOffset, sizeof(long long int), 1, fp);
    fwrite(&c2->nroRegRem, sizeof(int), 1, fp);
}

// Lê cabeçalho tipo 2 de arquivo já aberto com permissão para leitura
void ler_cabecalho2_de_arquivo(cabecalho2_t *c2, FILE *fp) {
    fseek(fp, 0, SEEK_SET);
    fread(&c2->status, sizeof(char), 1,fp);
    fread(&c2->topo, sizeof(long long int), 1, fp);
    fseek(fp,169,SEEK_CUR);
    fread(&c2->proxByteOffset, sizeof(long long int), 1, fp);
    fread(&c2->nroRegRem, sizeof(int), 1, fp);
}

void destruir_cabecalho2(cabecalho2_t *c2) {
    free(c2);
}