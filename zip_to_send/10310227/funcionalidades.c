#include <stdio.h>
#include <stdlib.h>

#include "funcionalidades.h"
#include "cabecalhos.h"
#include "registros.h"
#include "str.h"

void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	int fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void csv_para_binario1(string_t csv, string_t binario_saida) {
    int has_EOF = 0;
    FILE *arq_csv = NULL;
    FILE *arq_saida = NULL;

    arq_csv = fopen(csv, "r"); 
    arq_saida = fopen(binario_saida,"wb");

    if (!arq_csv || !arq_saida) {
        printf("Falha no processamento do arquivo.\n");

        if (arq_csv) fclose(arq_csv);
        if (arq_saida) fclose(arq_saida);

        return;
    }

    // Escrever cabeçalho inicializado com valores padrão:
    cabecalho1_t *c1 = criar_cabecalho1();
    escrever_cabecalho1_em_arquivo(c1, arq_saida);    
    
    // Ler e destruir primeira linha de header do csv:
    string_t header_csv = read_line(arq_csv, &has_EOF);
    destroy_string(header_csv);

    while(!has_EOF) {
        string_t linha = read_line(arq_csv, &has_EOF);

        // Se tinha EOF finalizando a linha, mas não leu nada, arquivo acabou;
        // Se tinha EOF finalizando a linha, mas leu algo, é a última linha, roda só mais uma vez:
        if (has_EOF && string_length(linha) == 0) {
            destroy_string(linha);
            break;
        }

        // Quebrar a linha nos campos separados por vírgula:
        int n_campos = 0;
        string_t *campos = split_string(linha, ',', &n_campos);
        destroy_string(linha);

        // Interpretar os 7 campos do CSV em formato esperado:        
        registro1_t *reg1 = criar_registro1();
        set_id1(reg1, atoi(campos[0]));
        if (string_length(campos[1]) > 0)
            set_ano1(reg1, atoi(campos[1]));
        set_cidade1(reg1, campos[2]);
        if (string_length(campos[3]) > 0)
            set_qtt1(reg1, atoi(campos[3]));
        set_sigla1(reg1, campos[4]);
        set_marca1(reg1, campos[5]);
        set_modelo1(reg1, campos[6]);

        // Escrever registro no arquivo:
        escrever_registro1_em_arquivo(reg1, arq_saida);

        // Atualizar próximo RRN no cabeçalho em RAM:
        int novoProxRRN = get_proxRRN1(c1) + 1;
        set_proxRRN1(c1, novoProxRRN);

        // Liberar memória:
        destruir_registro1(reg1, 0);
        destroy_string_array(campos, n_campos);
    }

    // Atualizar status no cabeçalho em RAM:
    set_status1(c1,'1'); // consistente após todas as alterações

    // Gravar cabeçalho com todas as alterações no arquivo final:
    escrever_cabecalho1_em_arquivo(c1, arq_saida);

    destruir_cabecalho1(c1);
    fclose(arq_csv);
    fclose(arq_saida);
}

void csv_para_binario2(string_t csv, string_t binario_saida) {
    int has_EOF = 0;
    FILE *arq_csv = NULL;
    FILE *arq_saida = NULL;

    arq_csv = fopen(csv, "r"); 
    arq_saida = fopen(binario_saida,"wb");

    if (!arq_csv || !arq_saida) {
        printf("Falha no processamento do arquivo.\n");

        return;
    }

    // Escrever cabeçalho inicializado com valores padrão:
    cabecalho2_t *c2 = criar_cabecalho2();
    escrever_cabecalho2_em_arquivo(c2, arq_saida);    
    
    // Setando próximo byteoffset como depois do cabeçalho:
    set_proxByteOffset2(c2, TAM_CAB_2);

    // Ler e destruir primeira linha de header do csv:
    string_t header_csv = read_line(arq_csv, &has_EOF);
    destroy_string(header_csv);

    while(!has_EOF) {
        string_t linha = read_line(arq_csv, &has_EOF);

        // Se tinha EOF finalizando a linha, mas não leu nada, arquivo acabou;
        // Se tinha EOF finalizando a linha, mas leu algo, é a última linha, roda só mais uma vez:
        if (has_EOF && string_length(linha) == 0) {
            destroy_string(linha);
            break;
        }

        // Quebrar a linha nos campos separados por vírgula:
        int n_campos = 0;
        string_t *campos = split_string(linha, ',', &n_campos);
        destroy_string(linha);

        // Interpretar os 7 campos do CSV em formato esperado:      
        registro2_t *reg2 = criar_registro2();
        set_id2(reg2, atoi(campos[0]));
        if (string_length(campos[1]) > 0)
            set_ano2(reg2, atoi(campos[1]));
        set_cidade2(reg2, campos[2]);
        if (string_length(campos[3]) > 0)
            set_qtt2(reg2, atoi(campos[3]));
        set_sigla2(reg2, campos[4]);
        set_marca2(reg2, campos[5]);
        set_modelo2(reg2, campos[6]);

        // Escrever registro no arquivo e guardar seu tamanho total:
        int tam_reg = escrever_registro2_em_arquivo(reg2, arq_saida);

        // Atualizar próximo byteoffset no cabeçalho em RAM:
        int novoProxByteOffset = get_proxByteOffset2(c2) + tam_reg;
        set_proxByteOffset2(c2, novoProxByteOffset);

        // Liberar memória:
        destruir_registro2(reg2, 0);
        destroy_string_array(campos, n_campos);
    }

    // Atualizar status no cabeçalho em RAM:
    set_status2(c2,'1'); // consistente após todas as alterações

    // Gravar cabeçalho com todas as alterações no arquivo final:
    escrever_cabecalho2_em_arquivo(c2, arq_saida);

    destruir_cabecalho2(c2);
    fclose(arq_csv);
    fclose(arq_saida);
}

void funcionalidade1(string_t tipo_do_arquivo, string_t csv, string_t binario_saida) {
    if (compare_strings_case_sensitive(tipo_do_arquivo, "tipo1") == 0) {
        csv_para_binario1(csv, binario_saida);
        binarioNaTela(binario_saida);
    } else if (compare_strings_case_sensitive(tipo_do_arquivo, "tipo2") == 0) {
        csv_para_binario2(csv, binario_saida);
        binarioNaTela(binario_saida);
    }
}

void funcionalidade2(string_t tipo_do_arquivo, string_t binario_entrada) {
    FILE *arq_entrada = NULL;

    arq_entrada = fopen(binario_entrada,"rb");

    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");

        return;
    }

    if (compare_strings_case_sensitive(tipo_do_arquivo, "tipo1") == 0) {
        cabecalho1_t *c1 = criar_cabecalho1();

        ler_cabecalho1_de_arquivo(c1, arq_entrada);

        int proxRRN = get_proxRRN1(c1);
        char status = get_status1(c1);

        destruir_cabecalho1(c1);
       
        if (status == '0') {
            printf("Falha no processamento do arquivo.\n");
            fclose(arq_entrada);

            return;
        }

        if (proxRRN == 0) {
            printf("Registro inexistente\n");
            fclose(arq_entrada);

            return;
        }

        for (int i = 0; i < proxRRN; i++) {
            registro1_t *reg1 = criar_registro1();

            ler_registro1(reg1, arq_entrada);
            imprimir_reg1(reg1);
            destruir_registro1(reg1, 1);
        }
    } else if (compare_strings_case_sensitive(tipo_do_arquivo, "tipo2") == 0) {
        cabecalho2_t *c2 = criar_cabecalho2();

        ler_cabecalho2_de_arquivo(c2, arq_entrada);

        long long int proxByteOffset = get_proxByteOffset2(c2);
        
        char status = get_status2(c2);

        destruir_cabecalho2(c2);

        if (status == '0') {
            printf("Falha no processamento do arquivo.\n");
            fclose(arq_entrada);

            return;
        }

        if (proxByteOffset <= TAM_CAB_2) {
            printf("Registro inexistente\n");
            fclose(arq_entrada);

            return;
        }

        long long int byteOffset_atual = TAM_CAB_2;

        while (byteOffset_atual < proxByteOffset) {
            registro2_t *reg2 = criar_registro2();

            int tam_reg = ler_registro2(reg2, arq_entrada);

            byteOffset_atual += tam_reg;

            imprimir_reg2(reg2);
            destruir_registro2(reg2, 1);
        }
    }

    fclose(arq_entrada);
}

void funcionalidade4(string_t tipo_de_arquivo, string_t binario_entrada, int RRN) {
    FILE *arq_entrada = NULL;

    arq_entrada = fopen(binario_entrada, "rb");

    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");

        return;
    }

    cabecalho1_t *c1 = criar_cabecalho1();

    ler_cabecalho1_de_arquivo(c1, arq_entrada);

    int proxRRN = get_proxRRN1(c1);
    char status = get_status1(c1);

    destruir_cabecalho1(c1);

    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");

        fclose(arq_entrada);
        
        return;
    }

    if (RRN >= proxRRN || RRN < 0) {
        printf("Registro inexistente.\n");

        fclose(arq_entrada);
        
        return;
    }
    
    long long int byte_offset = TAM_CAB_1 + RRN * TAM_REG1;

    fseek(arq_entrada, byte_offset, SEEK_SET);

    registro1_t *reg1 = criar_registro1();

    ler_registro1(reg1, arq_entrada);
    imprimir_reg1(reg1);

    destruir_registro1(reg1, 1);
    fclose(arq_entrada);
}