#include <stdio.h>
#include <stdlib.h>

#include "registros.h"
#include "cabecalhos.h"
#include "str.h"

struct registro1_ {
    char removido;
    int prox;
    int id;
    int ano;
    int qtt;
    string_t sigla;
    string_t cidade;
    string_t marca;
    string_t modelo;
};

struct registro2_ {
    char removido;
    long long int prox;
    int id;
    int ano;
    int qtt;
    string_t sigla;
    string_t cidade;
    string_t marca;
    string_t modelo;
};

// Aloca e retorna um array de caracteres lixo ('$') de tamanho indicado (sem '\0')
char *cria_lixo(int tamLixo) {
    char *lixo = (char *)malloc(tamLixo * sizeof(char));

    for (int i = 0; i < tamLixo; i++) {
        lixo[i] = '$';
    }

    return lixo;
}

registro1_t *criar_registro1() {
    registro1_t *reg1 = (registro1_t *)malloc(1 * sizeof(registro1_t));

    reg1->removido = '0';
    reg1->prox = -1;
    reg1->id = -1;
    reg1->ano = -1;
    reg1->qtt = -1;
    reg1->cidade = NULL;
    reg1->marca = NULL;
    reg1->modelo = NULL;

    return reg1;
}

registro2_t *criar_registro2() {
    registro2_t *reg2 = (registro2_t *)malloc(1 * sizeof(registro2_t));

    reg2->removido = '0';
    reg2->prox = -1;
    reg2->id = -1;
    reg2->ano = -1;
    reg2->qtt = -1;
    reg2->cidade = NULL;
    reg2->marca = NULL;
    reg2->modelo = NULL;

    return reg2;
}

void destruir_registro1(registro1_t *reg1, int liberar_strings) {
    if (reg1) {
        if (liberar_strings) {
            destroy_string(reg1->sigla);
            destroy_string(reg1->cidade);
            destroy_string(reg1->marca);
            destroy_string(reg1->modelo);
        }

        free(reg1);
    }
}

void destruir_registro2(registro2_t *reg2, int liberar_strings) {
    if (reg2) {
        if (liberar_strings) {
            destroy_string(reg2->sigla);
            destroy_string(reg2->cidade);
            destroy_string(reg2->marca);
            destroy_string(reg2->modelo);
        }

        free(reg2);
    }
}

void escrever_registro1_em_arquivo(registro1_t *reg1, FILE *fp) { 
        int tamanho_registro_atual = 0;

        // Adicionar campo de 1 byte 'removido':
        fwrite(&reg1->removido,sizeof(char),1,fp);
        tamanho_registro_atual += 1;

        // Adicionar campo de próximo removido:
        fwrite(&reg1->prox, sizeof(int), 1, fp);
        tamanho_registro_atual += sizeof(int);
    
        // Adicionar campo id, o qual é obrigatório:
        fwrite(&reg1->id, sizeof(int), 1, fp);
        tamanho_registro_atual += sizeof(int);

        // Adicionar campo opcional de tamanho fixo Ano de Fabricação:
        fwrite(&reg1->ano, sizeof(int), 1, fp);
        tamanho_registro_atual += sizeof(int);
       
        // Adicionar campo opcional de tamanho fixo Quantidade:
        fwrite(&reg1->qtt, sizeof(int), 1, fp);
        tamanho_registro_atual += sizeof(int);

        // Adicionar campo opcional de tamanho fixo Sigla do Estado:
        if (string_length(reg1->sigla) > 0) {
            fwrite(reg1->sigla, sizeof(char), 2, fp);
        } else {
            // caso campo de tamanho fixo 2 esteja vazio, completar com lixo
            char lixo[3] = "$$"; 
            fwrite(lixo, sizeof(char), 2, fp);
        }
        tamanho_registro_atual += 2;

        // Adicionar campo de tamanho variável e opcional Cidade, caso haja:
        int tamCidade = string_length(reg1->cidade);
        if (tamCidade > 0) {
            // primeiro, escrever tamanho do nome da cidade
            fwrite(&tamCidade, sizeof(int), 1, fp);
            tamanho_registro_atual += sizeof(int);

            // depois, o código do campo cidade
            char codC5 = '0'; // Código de Cidade = 0
            fwrite(&codC5, sizeof(char), 1, fp);
            tamanho_registro_atual += sizeof(char);

            // e então, a o nome da cidade em si
            fwrite(reg1->cidade, sizeof(char), tamCidade, fp);
            tamanho_registro_atual += tamCidade;
        }

        // Adicionar campo de tamanho variável e opcional Marca, caso haja:
        int tamMarca = string_length(reg1->marca);
        if (tamMarca > 0) {
            // primeiro, escrever o tamanho do nome da marca
            fwrite(&tamMarca, sizeof(int), 1, fp);
            tamanho_registro_atual += sizeof(int);

            // depois, o código do campo Marca
            char codC6 = '1'; // Código de Marca = 1
            fwrite(&codC6, sizeof(char), 1, fp);
            tamanho_registro_atual += sizeof(char);

            // e então, o nome da Marca em si
            fwrite(reg1->marca, sizeof(char), tamMarca, fp);
            tamanho_registro_atual += tamMarca;
        }

        // Adicionar campo de tamanho variável e opcional Modelo, caso haja:
        int tamModelo = string_length(reg1->modelo);
        if (tamModelo > 0) {
            // primeiro, escrever o tamanho do nome do modelo
            fwrite(&tamModelo, sizeof(int), 1, fp);
            tamanho_registro_atual += sizeof(int);

            // depois, o código do campo Modelo
            char codC7 = '2'; // Código de Modelo = 2
            fwrite(&codC7, sizeof(char), 1, fp);
            tamanho_registro_atual += sizeof(char);

            // e então, o nome do Modelo em si
            fwrite(reg1->modelo, sizeof(char), tamModelo, fp);
            tamanho_registro_atual += tamModelo;
        }

        // Completar resto do registro de tamanho fixo com lixo:
        char *lixo = cria_lixo(TAM_REG1 - tamanho_registro_atual);
        fwrite(lixo, sizeof(char), TAM_REG1 - tamanho_registro_atual, fp);
        free(lixo);
}

// Retorna tamanho do registro
int escrever_registro2_em_arquivo(registro2_t *reg2, FILE *fp) {
        // Contabilizar tamanho do registro depois do quinto byte:
        int tamanho_registro_atual = 0;
        tamanho_registro_atual += 8; // prox - long long int - 8 bytes
        tamanho_registro_atual += 4; // id - int - 4 bytes
        tamanho_registro_atual += 4; // ano - int - 4 bytes
        tamanho_registro_atual += 4; // qtt - int - 4 bytes
        tamanho_registro_atual += 2; // sigla - char[2] - 2 bytes

        int tamCidade = string_length(reg2->cidade);
        if (tamCidade > 0) {
            tamanho_registro_atual += 4; // tamCidade - int - 4 bytes
            tamanho_registro_atual += 1; // codC5 - char - 1 byte
            tamanho_registro_atual += tamCidade; // cidade - string - n bytes
        }

        int tamMarca = string_length(reg2->marca);
        if (tamMarca > 0) {
            tamanho_registro_atual += 4; // tamMarca - int - 4 bytes
            tamanho_registro_atual += 1; // codC6 - char - 1 byte
            tamanho_registro_atual += tamMarca; // marca - string - n bytes
        }

        int tamModelo = string_length(reg2->modelo);
        if (tamModelo > 0) {
            tamanho_registro_atual += 4; // tamModelo - int - 4 bytes
            tamanho_registro_atual += 1; // codC7 - char - 1 byte
            tamanho_registro_atual += tamModelo; // modelo - string - n bytes
        }

        // Adicionar campo de 1 byte 'removido':
        fwrite(&reg2->removido, sizeof(char), 1, fp);

        // Adicionar campo tamanho do registro:
        fwrite(&tamanho_registro_atual, sizeof(int), 1, fp);

        // Adicionar campo de próximo removido:
        fwrite(&reg2->prox, sizeof(long long int), 1, fp);
    
        // Adicionar campo id, o qual é obrigatório:
        fwrite(&reg2->id, sizeof(int), 1, fp);

        // Adicionar campo opcional de tamanho fixo Ano de Fabricação:
        fwrite(&reg2->ano, sizeof(int), 1, fp);
       
        // Adicionar campo opcional de tamanho fixo Quantidade:
        fwrite(&reg2->qtt, sizeof(int), 1, fp);

        // Adicionar campo opcional de tamanho fixo Sigla do Estado:
        if (string_length(reg2->sigla) > 0) {
            fwrite(reg2->sigla, sizeof(char), 2, fp);
        } else {
            // caso campo de tamanho fixo 2 esteja vazio, completar com lixo
            char lixo[3] = "$$"; 
            fwrite(lixo, sizeof(char), 2, fp);
        }

        // Adicionar campo de tamanho variável e opcional Cidade, caso haja:
        if (tamCidade > 0) {
            // primeiro, escrever tamanho do nome da cidade
            fwrite(&tamCidade, sizeof(int), 1, fp);

            // depois, o código do campo cidade
            char codC5 = '0'; // Código de Cidade = 0
            fwrite(&codC5, sizeof(char), 1, fp);

            // e então, a o nome da cidade em si
            fwrite(reg2->cidade, sizeof(char), tamCidade, fp);
        }

        // Adicionar campo de tamanho variável e opcional Marca, caso haja:
        if (tamMarca > 0) {
            // primeiro, escrever o tamanho do nome da marca
            fwrite(&tamMarca, sizeof(int), 1, fp);

            // depois, o código do campo Marca
            char codC6 = '1'; // Código de Marca = 1
            fwrite(&codC6, sizeof(char), 1, fp);

            // e então, o nome da Marca em si
            fwrite(reg2->marca, sizeof(char), tamMarca, fp);
        }

        // Adicionar campo de tamanho variável e opcional Modelo, caso haja:
        if (tamModelo > 0) {
            // primeiro, escrever o tamanho do nome do modelo
            fwrite(&tamModelo, sizeof(int), 1, fp);

            // depois, o código do campo Modelo
            char codC7 = '2'; // Código de Modelo = 2
            fwrite(&codC7, sizeof(char), 1, fp);

            // e então, o nome do Modelo em si
            fwrite(reg2->modelo, sizeof(char), tamModelo, fp);
        }

        // Retorna o tamanho total do registro, incluindo os 5 bytes iniciais
        return tamanho_registro_atual + 5; 
}

void ler_registro1(registro1_t *reg1, FILE *fp) {
    int tam_reg = 0;

    fread(&reg1->removido, sizeof(char), 1, fp);
    fread(&reg1->prox, sizeof(int), 1, fp);
    fread(&reg1->id, sizeof(int), 1, fp);
    fread(&reg1->ano, sizeof(int), 1, fp);
    fread(&reg1->qtt, sizeof(int), 1, fp);

    // Alocando espaço para sigla e lendo:
    string_t sigla = (string_t)malloc(3 * sizeof(symbol_t));
    fread(sigla, sizeof(char), 2, fp);
    sigla[2] = '\0';
    reg1->sigla = sigla;

    tam_reg += 19; // Soma dos campos de tamanho fixo

    int n_lidos = 0;
    int proximo_campo_valido = 1;

    // Continua lendo enquanto:
        // * Foram lidos menos de 3 campos e;
        // * Tem pelo menos mais 5 bytes cabendo no tamanho do registro
        // * O código do próximo campos testado é válido
    while (n_lidos < 3 && tam_reg + 5 < TAM_REG1 && proximo_campo_valido) {
        int tamanho_do_campo;
        fread(&tamanho_do_campo, sizeof(int), 1, fp);
        char codigo_campo;
        fread(&codigo_campo, sizeof(char), 1, fp);
        
        tam_reg += (4 + 1);

        if (codigo_campo != '0' && codigo_campo != '1' && codigo_campo != '2') {
            proximo_campo_valido = 0;
            continue;
        }

        n_lidos += 1; // Mais um campo sendo lido

        string_t campo = (string_t)malloc((tamanho_do_campo + 1) * sizeof(symbol_t));
        fread(campo, sizeof(char), tamanho_do_campo, fp);
        campo[tamanho_do_campo] = '\0';

        tam_reg += tamanho_do_campo;

        if (codigo_campo == '0') {
            reg1->cidade = campo;
        } else if (codigo_campo == '1') {
            reg1->marca = campo;
        } else if (codigo_campo == '2') {
            reg1->modelo = campo;
        }
    }

    // Posicionando o ponteiro no possível próximo registro:
    fseek(fp, (TAM_REG1 - tam_reg), SEEK_CUR);
}

int ler_registro2(registro2_t *reg2, FILE *fp) {
    int tam_reg = 0;
    int tam_total_reg = 0;

    fread(&reg2->removido, sizeof(char), 1, fp);
    fread(&tam_total_reg, sizeof(int), 1, fp);
    fread(&reg2->prox, sizeof(long long int), 1, fp);
    fread(&reg2->id, sizeof(int), 1, fp);
    fread(&reg2->ano, sizeof(int), 1, fp);
    fread(&reg2->qtt, sizeof(int), 1, fp);

    tam_total_reg += 5; // Incluindo os 5 primeiros bytes iniciais

    // Alocando espaço para sigla e lendo:
    string_t sigla = (string_t)malloc(3 * sizeof(symbol_t));
    fread(sigla, sizeof(char), 2, fp);
    sigla[2] = '\0';
    reg2->sigla = sigla;

    tam_reg += 27; // Soma dos campos de tamanho fixo

    int n_lidos = 0;
    int proximo_campo_valido = 1;

    // Continua lendo enquanto:
        // * Foram lidos menos de 3 campos e;
        // * Tem pelo menos mais 5 bytes cabendo no tamanho do registro
        // * O código do próximo campos testado é válido
    while (n_lidos < 3 && tam_reg + 5 < tam_total_reg && proximo_campo_valido) {
        int tamanho_do_campo;
        fread(&tamanho_do_campo, sizeof(int), 1, fp);
        char codigo_campo;
        fread(&codigo_campo, sizeof(char), 1, fp);
        
        tam_reg += (4 + 1);

        if (codigo_campo != '0' && codigo_campo != '1' && codigo_campo != '2') {
            proximo_campo_valido = 0;
            continue;
        }

        n_lidos += 1; // Mais um campo sendo lido

        string_t campo = (string_t)malloc((tamanho_do_campo + 1) * sizeof(symbol_t));
        fread(campo, sizeof(char), tamanho_do_campo, fp);
        campo[tamanho_do_campo] = '\0';

        tam_reg += tamanho_do_campo;

        if (codigo_campo == '0') {
            reg2->cidade = campo;
        } else if (codigo_campo == '1') {
            reg2->marca = campo;
        } else if (codigo_campo == '2') {
            reg2->modelo = campo;
        }
    }

    // Posicionando o ponteiro no possível próximo registro:
    fseek(fp, (tam_total_reg - tam_reg), SEEK_CUR);

    return tam_total_reg;
}

void imprimir_reg1(registro1_t *reg1) {
    printf("MARCA DO VEICULO: ");
    if (reg1->marca && string_length(reg1->marca) > 0) {
        print_string(reg1->marca);
    } else {
        printf("NAO PREENCHIDO");
    }
    printf("\nMODELO DO VEICULO: ");
    if (reg1->modelo && string_length(reg1->modelo) > 0) {
        print_string(reg1->modelo);
    } else {
        printf("NAO PREENCHIDO");
    }
    printf("\nANO DE FABRICACAO: ");
    if (reg1->ano != -1) {
        printf("%d", reg1->ano);
    } else {
        printf("NAO PREENCHIDO");
    }
    printf("\nNOME DA CIDADE: ");
    if (reg1->cidade && string_length(reg1->cidade) > 0) {
        print_string(reg1->cidade);
    } else {
        printf("NAO PREENCHIDO");
    }
    printf("\nQUANTIDADE DE VEICULOS: ");
    if (reg1->qtt != -1) {
        printf("%d", reg1->qtt);
    } else {
        printf("NAO PREENCHIDO");
    }

    new_lines(2);
}

void imprimir_reg2(registro2_t *reg2) {
    printf("MARCA DO VEICULO: ");
    if (reg2->marca && string_length(reg2->marca) > 0) {
        print_string(reg2->marca);
    } else {
        printf("NAO PREENCHIDO");
    }
    printf("\nMODELO DO VEICULO: ");
    if (reg2->modelo && string_length(reg2->modelo) > 0) {
        print_string(reg2->modelo);
    } else {
        printf("NAO PREENCHIDO");
    }
    printf("\nANO DE FABRICACAO: ");
    if (reg2->ano != -1) {
        printf("%d", reg2->ano);
    } else {
        printf("NAO PREENCHIDO");
    }
    printf("\nNOME DA CIDADE: ");
    if (reg2->cidade && string_length(reg2->cidade) > 0) {
        print_string(reg2->cidade);
    } else {
        printf("NAO PREENCHIDO");
    }
    printf("\nQUANTIDADE DE VEICULOS: ");
    if (reg2->qtt != -1) {
        printf("%d", reg2->qtt);
    } else {
        printf("NAO PREENCHIDO");
    }

    new_lines(2);
}

// Getters e Setters:

// para tipo 1:

void set_removido1(registro1_t *reg1, char removido) {
    reg1->removido = removido;
}

char get_removido1(registro1_t *reg1) {
    return reg1->removido;
}

void set_prox1(registro1_t *reg1, int prox) {
    reg1->prox = prox;
}

int get_prox1(registro1_t *reg1) {
    return reg1->prox;
}

void set_id1(registro1_t *reg1, int id) {
    reg1->id = id;
}

int get_id1(registro1_t *reg1) {
    return reg1->id;
}

void set_ano1(registro1_t *reg1, int ano) {
    reg1->ano = ano;
}

int get_ano1(registro1_t *reg1) {
    return reg1->ano;
}

void set_qtt1(registro1_t *reg1, int qtt) {
    reg1->qtt = qtt;
}

int get_qtt1(registro1_t *reg1) {
    return reg1->qtt;
}

void set_sigla1(registro1_t *reg1, string_t sigla) {
    reg1->sigla = sigla;
}

string_t get_sigla1(registro1_t *reg1) {
    return reg1->sigla;
}

void set_cidade1(registro1_t *reg1, string_t cidade) {
    reg1->cidade = cidade;
}

string_t get_cidade1(registro1_t *reg1) {
    return reg1->cidade;
}

void set_marca1(registro1_t *reg1, string_t marca) {
    reg1->marca = marca;
}

string_t get_marca1(registro1_t *reg1) {
    return reg1->marca;
}

void set_modelo1(registro1_t *reg1, string_t modelo) {
    reg1->modelo = modelo;
}

string_t get_modelo1(registro1_t *reg1) {
    return reg1->sigla;
}

// para tipo 2:

void set_removido2(registro2_t *reg2, char removido) {
    reg2->removido = removido;
}

char get_removido2(registro2_t *reg2) {
    return reg2->removido;
}

void set_prox2(registro2_t *reg2, long long int prox) {
    reg2->prox = prox;
}

long long int get_prox2(registro2_t *reg2) {
    return reg2->prox;
}

void set_id2(registro2_t *reg2, int id) {
    reg2->id = id;
}

int get_id2(registro2_t *reg2) {
    return reg2->id;
}

void set_ano2(registro2_t *reg2, int ano) {
    reg2->ano = ano;
}

int get_ano2(registro2_t *reg2) {
    return reg2->ano;
}

void set_qtt2(registro2_t *reg2, int qtt) {
    reg2->qtt = qtt;
}

int get_qtt2(registro2_t *reg2) {
    return reg2->qtt;
}

void set_sigla2(registro2_t *reg2, string_t sigla) {
    reg2->sigla = sigla;
}

string_t get_sigla2(registro2_t *reg2) {
    return reg2->sigla;
}

void set_cidade2(registro2_t *reg2, string_t cidade) {
    reg2->cidade = cidade;
}

string_t get_cidade2(registro2_t *reg2) {
    return reg2->cidade;
}

void set_marca2(registro2_t *reg2, string_t marca) {
    reg2->marca = marca;
}

string_t get_marca2(registro2_t *reg2) {
    return reg2->marca;
}

void set_modelo2(registro2_t *reg2, string_t modelo) {
    reg2->modelo = modelo;
}

string_t get_modelo2(registro2_t *reg2) {
    return reg2->sigla;
}