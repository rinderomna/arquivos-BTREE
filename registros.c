#include <stdio.h>
#include <stdlib.h>

#include "registros.h"
#include "cabecalhos.h"
#include "str.h"

struct registro_ {
    char removido;
    int proxRRN;
    long long int proxByteOffset;
    int id;
    int ano;
    int qtt;
    string_t sigla;
    string_t cidade;
    string_t marca;
    string_t modelo;
};

registro_t *criar_registro() {
    registro_t *reg = (registro_t *)malloc(1 * sizeof(registro_t));

    reg->removido = '0';
    reg->proxRRN = -1;
    reg->proxByteOffset = -1;
    reg->id = -1;
    reg->ano = -1;
    reg->qtt = -1;
    reg->cidade = NULL;
    reg->marca = NULL;
    reg->modelo = NULL;

    return reg;
}

void destruir_registro(registro_t *reg, int liberar_strings) {
    if (reg) {
        if (liberar_strings) {
            if (reg->sigla) destroy_string(reg->sigla);
            if (reg->cidade) destroy_string(reg->cidade);
            if (reg->marca) destroy_string(reg->marca);
            if (reg->modelo) destroy_string(reg->modelo);
        }

        free(reg);
    }
}

void escrever_registro1_em_arquivo(registro_t *reg1, FILE *fp) { 
    int tamanho_registro_atual = 0;

    // Adicionar campo de 1 byte 'removido':
    fwrite(&reg1->removido, sizeof(char), 1, fp);
    tamanho_registro_atual += 1;

    // Adicionar campo de próximo removido:
    fwrite(&reg1->proxRRN, sizeof(int), 1, fp);
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
    int tamCidade = 0;
    if (reg1->cidade) tamCidade = string_length(reg1->cidade);
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
    int tamMarca = 0;
    if (reg1->marca) tamMarca = string_length(reg1->marca);
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
    int tamModelo = 0;
    if (reg1->modelo) tamModelo = string_length(reg1->modelo);
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
int escrever_registro2_em_arquivo(registro_t *reg2, FILE *fp) {
    // Contabilizar tamanho do registro depois do quinto byte:
    int tamanho_registro_atual = 0;
    tamanho_registro_atual += 8; // prox - long long int - 8 bytes
    tamanho_registro_atual += 4; // id - int - 4 bytes
    tamanho_registro_atual += 4; // ano - int - 4 bytes
    tamanho_registro_atual += 4; // qtt - int - 4 bytes
    tamanho_registro_atual += 2; // sigla - char[2] - 2 bytes

    int tamCidade = 0;
    if (reg2->cidade) tamCidade = string_length(reg2->cidade);
    if (tamCidade > 0) {
        tamanho_registro_atual += 4; // tamCidade - int - 4 bytes
        tamanho_registro_atual += 1; // codC5 - char - 1 byte
        tamanho_registro_atual += tamCidade; // cidade - string - n bytes
    }

    int tamMarca = 0;
    if (reg2->marca) tamMarca = string_length(reg2->marca);
    if (tamMarca > 0) {
        tamanho_registro_atual += 4; // tamMarca - int - 4 bytes
        tamanho_registro_atual += 1; // codC6 - char - 1 byte
        tamanho_registro_atual += tamMarca; // marca - string - n bytes
    }

    int tamModelo = 0;
    if (reg2->modelo) tamModelo = string_length(reg2->modelo);
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
    fwrite(&reg2->proxByteOffset, sizeof(long long int), 1, fp);

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

// Escrever registro tipo 2 em cima de registro já existente,
// mantendo indicador de tamanho anterior e completando com lixo ('$')
void reescrever_registro2_em_arquivo(registro_t *reg2, int tam_registro_anterior, FILE *fp) {
    int tamanho_registro_atual = tamanho_total_do_registro(2, reg2);

    // Adicionar campo de 1 byte 'removido':
    fwrite(&reg2->removido, sizeof(char), 1, fp);

    // Pular o campo de tamanho do registro, pois manterá o do anterior que é maior:
    fseek(fp, sizeof(int), SEEK_CUR);

    // Adicionar campo de próximo removido:
    fwrite(&reg2->proxByteOffset, sizeof(long long int), 1, fp);

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
    int tamCidade = 0;
    if (reg2->cidade) tamCidade = string_length(reg2->cidade);
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
    int tamMarca = 0;
    if (reg2->marca) tamMarca = string_length(reg2->marca);
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
    int tamModelo = 0;
    if (reg2->modelo) tamModelo = string_length(reg2->modelo);
    if (tamModelo > 0) {
        // primeiro, escrever o tamanho do nome do modelo
        fwrite(&tamModelo, sizeof(int), 1, fp);

        // depois, o código do campo Modelo
        char codC7 = '2'; // Código de Modelo = 2
        fwrite(&codC7, sizeof(char), 1, fp);

        // e então, o nome do Modelo em si
        fwrite(reg2->modelo, sizeof(char), tamModelo, fp);
    }

    // Escrever lixo no final
    char *lixo = cria_lixo(tam_registro_anterior - tamanho_registro_atual);
    fwrite(lixo, sizeof(char), tam_registro_anterior - tamanho_registro_atual, fp);
    free(lixo);
}

void escrever_registro_em_posicao(int tipo_do_arquivo, registro_t *reg, long long int posicao, FILE *fp) {
    if (tipo_do_arquivo != 1 && tipo_do_arquivo != 2) return;

    long long int byte_offset = 0;

    if (tipo_do_arquivo == 1) {
        int RRN = (int)posicao;
        byte_offset = TAM_CAB_1 + RRN * TAM_REG1;
    } else {
        byte_offset = posicao;
    }

    fseek(fp, byte_offset, SEEK_SET);

    escrever_registro1_em_arquivo(reg, fp);
}

registro_t *get_registro_em_posicao(int tipo_do_arquivo, long long int posicao, FILE *fp) {
    if (tipo_do_arquivo != 1 && tipo_do_arquivo != 2) return NULL;

    long long int byte_offset = 0;

    if (tipo_do_arquivo == 1) {
        int RRN = (int)posicao;
        byte_offset = TAM_CAB_1 + RRN * TAM_REG1;
    } else {
        byte_offset = posicao;
    }

    fseek(fp, byte_offset, SEEK_SET);
    registro_t *reg = criar_registro();
    
    ler_registro(reg, tipo_do_arquivo, fp);

    return reg;
}

int ler_registro(registro_t *reg, int tipo_do_registro, FILE *fp) {
    int tam_atual_reg1 = 0;
    int tam_atual_reg2 = 0;
    int tam_total_reg2 = 0;

    fread(&reg->removido, sizeof(char), 1, fp);
    if (tipo_do_registro == 1) {
        fread(&reg->proxRRN, sizeof(int), 1, fp);
    } else if (tipo_do_registro == 2) {
        fread(&tam_total_reg2, sizeof(int), 1, fp);
        fread(&reg->proxByteOffset, sizeof(long long int), 1, fp);
    }
    fread(&reg->id, sizeof(int), 1, fp);
    fread(&reg->ano, sizeof(int), 1, fp);
    fread(&reg->qtt, sizeof(int), 1, fp);

    tam_total_reg2 += 5; // Incluindo os 5 primeiros bytes iniciais

    // Alocando espaço para sigla e lendo:
    string_t sigla = (string_t)malloc(3 * sizeof(symbol_t));
    fread(sigla, sizeof(char), 2, fp);
    sigla[2] = '\0';
    reg->sigla = sigla;

    tam_atual_reg1 += 19; // Soma dos campos de tamanho fixo
    tam_atual_reg2 += 27; // Soma dos campos de tamanho fixo

    int n_lidos = 0;
    int proximo_campo_valido = 1;

    char codigo_campo = ' ';

    // Continua lendo enquanto:
        // * Foram lidos menos de 3 campos e;
        // * Tem pelo menos mais 5 bytes cabendo no tamanho do registro
        // * O código do próximo campo testado é válido
    while (n_lidos < 3 &&
            ((tam_atual_reg1 + 5 < TAM_REG1 && tipo_do_registro == 1) ||
             (tam_atual_reg2 + 5 < tam_total_reg2 && tipo_do_registro == 2)) &&
            proximo_campo_valido) {
        int tamanho_do_campo = 0;
        fread(&tamanho_do_campo, sizeof(int), 1, fp);
        codigo_campo = ' ';
        fread(&codigo_campo, sizeof(char), 1, fp);
        
        tam_atual_reg1 += (4 + 1);
        tam_atual_reg2 += (4 + 1);

        if (codigo_campo != '0' && codigo_campo != '1' && codigo_campo != '2') {
            proximo_campo_valido = 0;
            continue;
        }

        n_lidos += 1; // Mais um campo sendo lido

        string_t campo = (string_t)malloc((tamanho_do_campo + 1) * sizeof(symbol_t));
        fread(campo, sizeof(char), tamanho_do_campo, fp);
        campo[tamanho_do_campo] = '\0';

        tam_atual_reg1 += tamanho_do_campo;
        tam_atual_reg2 += tamanho_do_campo;

        if (codigo_campo == '0') {
            reg->cidade = campo;
        } else if (codigo_campo == '1') {
            reg->marca = campo;
        } else if (codigo_campo == '2') {
            reg->modelo = campo;
        }
    }

    int tamanho_total_registro = 0;
    // Posicionando o ponteiro no possível próximo registro:
    if (tipo_do_registro == 1) {
        fseek(fp, (TAM_REG1 - tam_atual_reg1), SEEK_CUR);
        tamanho_total_registro = TAM_REG1;
    } else if (tipo_do_registro == 2) {
        fseek(fp, (tam_total_reg2 - tam_atual_reg2), SEEK_CUR);
        tamanho_total_registro = tam_total_reg2;
    }

    return tamanho_total_registro;
}

void imprimir_registro(registro_t *reg) {
    printf("MARCA DO VEICULO: ");
    if (reg->marca && string_length(reg->marca) > 0) {
        print_string(reg->marca);
    } else {
        printf("NAO PREENCHIDO");
    }
    printf("\nMODELO DO VEICULO: ");
    if (reg->modelo && string_length(reg->modelo) > 0) {
        print_string(reg->modelo);
    } else {
        printf("NAO PREENCHIDO");
    }
    printf("\nANO DE FABRICACAO: ");
    if (reg->ano != -1) {
        printf("%d", reg->ano);
    } else {
        printf("NAO PREENCHIDO");
    }
    printf("\nNOME DA CIDADE: ");
    if (reg->cidade && string_length(reg->cidade) > 0) {
        print_string(reg->cidade);
    } else {
        printf("NAO PREENCHIDO");
    }
    printf("\nQUANTIDADE DE VEICULOS: ");
    if (reg->qtt != -1) {
        printf("%d", reg->qtt);
    } else {
        printf("NAO PREENCHIDO");
    }

    new_lines(2);
}

int tamanho_total_do_registro(int tipo_do_arquivo, registro_t *reg) {
    if (tipo_do_arquivo == 1) {
        return TAM_REG1;
    } else if (tipo_do_arquivo == 2) {
        int tam_reg = 27; // campos fixos iniciais

        if (reg->cidade && string_length(reg->cidade) > 0) {
            tam_reg += 5 + string_length(reg->cidade);
        }

        if (reg->marca && string_length(reg->marca) > 0) {
            tam_reg += 5 + string_length(reg->marca);
        }

        if (reg->modelo && string_length(reg->modelo) > 0) {
            tam_reg += 5 + string_length(reg->modelo);
        }

        return tam_reg;
    }

    return -1;
}

// Getters e Setters:

void set_removido(registro_t *reg, char removido) {
    reg->removido = removido;
}

char get_removido(registro_t *reg) {
    return reg->removido;
}

void set_proxRRN_removido(registro_t *reg, int proxRRN) {
    reg->proxRRN = proxRRN;
}

int get_proxRRN_removido(registro_t *reg) {
    return reg->proxRRN;
}

void set_proxByteOffset_removido(registro_t *reg, long long int proxByteOffset) {
    reg->proxByteOffset = proxByteOffset;
}

long long int get_proxByteOffset_removido(registro_t *reg) {
    return reg->proxByteOffset;
}

void set_id(registro_t *reg, int id) {
    reg->id = id;
}

int get_id(registro_t *reg) {
    return reg->id;
}

void set_ano(registro_t *reg, int ano) {
    reg->ano = ano;
}

int get_ano(registro_t *reg) {
    return reg->ano;
}

void set_qtt(registro_t *reg, int qtt) {
    reg->qtt = qtt;
}

int get_qtt(registro_t *reg) {
    return reg->qtt;
}

void set_sigla(registro_t *reg, string_t sigla) {
    reg->sigla = sigla;
}

string_t get_sigla(registro_t *reg) {
    return reg->sigla;
}

void set_cidade(registro_t *reg, string_t cidade) {
    reg->cidade = cidade;
}

string_t get_cidade(registro_t *reg) {
    return reg->cidade;
}

void set_marca(registro_t *reg, string_t marca) {
    reg->marca = marca;
}

string_t get_marca(registro_t *reg) {
    return reg->marca;
}

void set_modelo(registro_t *reg, string_t modelo) {
    reg->modelo = modelo;
}

string_t get_modelo(registro_t *reg) {
    return reg->modelo;
}