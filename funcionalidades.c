#include <stdio.h>
#include <stdlib.h>

#include "funcionalidades.h"
#include "cabecalhos.h"
#include "registros.h"
#include "str.h"

void binarioNaTela(string_t nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

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

void funcionalidade1(int tipo_do_arquivo, string_t csv, string_t binario_saida) {
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
    cabecalho_t *cabecalho = criar_cabecalho();
    escrever_cabecalho_em_arquivo(cabecalho, tipo_do_arquivo, arq_saida);  
    
    // Setando próximo byteoffset como depois do cabeçalho caso seja tipo 2:
    set_proxByteOffset(cabecalho, TAM_CAB_2);
    
    int has_EOF = 0;
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
        registro_t *reg = criar_registro();
        set_id(reg, atoi(campos[0]));
        if (string_length(campos[1]) > 0) {
            set_ano(reg, atoi(campos[1]));
        }
        set_cidade(reg, campos[2]);
        if (string_length(campos[3]) > 0) {
            set_qtt(reg, atoi(campos[3]));
        }
        set_sigla(reg, campos[4]);
        set_marca(reg, campos[5]);
        set_modelo(reg, campos[6]);

        // Escrever registro no arquivo e atualizar próximo no cabeçalho em RAM:
        int tam_reg2 = 0;
        int novoProxRRN = 0;
        long long int novoProxByteOffset = 0;
        if (tipo_do_arquivo == 1) {
            escrever_registro1_em_arquivo(reg, arq_saida);
            novoProxRRN = get_proxRRN(cabecalho) + 1;
            set_proxRRN(cabecalho, novoProxRRN);
        } else if (tipo_do_arquivo == 2) {
            tam_reg2 = escrever_registro2_em_arquivo(reg, arq_saida);
            novoProxByteOffset = get_proxByteOffset(cabecalho) + tam_reg2;
            set_proxByteOffset(cabecalho, novoProxByteOffset);
        }

        // Liberar memória:
        destruir_registro(reg, 0);
        destroy_string_array(campos, n_campos);
    }

    // Atualizar status no cabeçalho em RAM:
    set_status(cabecalho, '1'); // consistente após todas as alterações

    // Gravar cabeçalho com todas as alterações no arquivo final:
    escrever_cabecalho_em_arquivo(cabecalho, tipo_do_arquivo, arq_saida);

    destruir_cabecalho(cabecalho);
    fclose(arq_csv);
    fclose(arq_saida);
    binarioNaTela(binario_saida);
}

void funcionalidade2(int tipo_do_arquivo, string_t binario_entrada) {
    FILE *arq_entrada = NULL;

    arq_entrada = fopen(binario_entrada,"rb");

    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");

        return;
    }

    cabecalho_t *cabecalho = criar_cabecalho();
    ler_cabecalho_de_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    char status = get_status(cabecalho);
    int proxRRN = get_proxRRN(cabecalho);
    long long int proxByteOffset = get_proxByteOffset(cabecalho);

    destruir_cabecalho(cabecalho);
    
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(arq_entrada);

        return;
    }

    if (tipo_do_arquivo == 1) {
        if (proxRRN == 0) {
            printf("Registro inexistente\n");
            fclose(arq_entrada);

            return;
        }

        for (int i = 0; i < proxRRN; i++) {
            registro_t *reg = criar_registro();

            ler_registro(reg, tipo_do_arquivo, arq_entrada);
            
            char removido = get_removido(reg);

            if (removido == '0') {
                imprimir_registro(reg);
            }

            destruir_registro(reg, 1);
        }
    } else if (tipo_do_arquivo == 2) {
        if (proxByteOffset <= TAM_CAB_2) {
            printf("Registro inexistente\n");
            fclose(arq_entrada);

            return;
        }

        long long int byteOffset_atual = TAM_CAB_2;

        while (byteOffset_atual < proxByteOffset) {
            registro_t *reg = criar_registro();

            int tam_reg = ler_registro(reg, tipo_do_arquivo, arq_entrada);
            byteOffset_atual += tam_reg;

            char removido = get_removido(reg);

            if (removido == '0') {
                imprimir_registro(reg);
            }

            destruir_registro(reg, 1);
        }
    }

    fclose(arq_entrada);
}


void funcionalidade3(int tipo_de_arquivo, string_t binario_entrada, int n) {
    FILE *arq_entrada = fopen(binario_entrada, "rb");
    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    string_t *nome_campos = (string_t *)malloc(n * sizeof(string_t));
    string_t *valor_campos = (string_t *)malloc(n * sizeof(string_t));

    for (int i = 0; i < n; i++) {
        nome_campos[i] = read_until(stdin, ' ', NULL);
        valor_campos[i] = read_until(stdin, '\n', NULL);
        remove_quotes(valor_campos[i]);
    }

    if (tipo_de_arquivo == 1) {
        busca_parametrizada1(arq_entrada, nome_campos, valor_campos, n);
    } else if (tipo_de_arquivo == 2){
        busca_parametrizada2(arq_entrada, nome_campos, valor_campos, n);
    } else {
        printf("Tipo de arquivo não existente\n");
    }

    fclose(arq_entrada);

    destroy_string_array(nome_campos, n);
    destroy_string_array(valor_campos, n);
}

void busca_parametrizada1(FILE *arq_entrada, string_t *nome_campos, string_t *valor_campos, int n) {
    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, 1, arq_entrada);

    int proxRRN = get_proxRRN(cabecalho);
    char status = get_status(cabecalho);

    destruir_cabecalho(cabecalho);
    
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");

        return;
    }

    if (proxRRN == 0) {
        printf("Registro inexistente\n");

        return;
    }

    int n_registros_encontrados = 0;
    string_t campos_validos[] = {"id", "ano", "qtt", "sigla", "cidade", "marca", "modelo"};

    for (int i = 0; i < proxRRN; i++) {
        registro_t *reg = criar_registro();

        ler_registro(reg, 1, arq_entrada);
        
        char removido = get_removido(reg);

        if (removido == '1') {
            destruir_registro(reg, 1);

            continue;
        }

        // Primeiramente, itero sobre o número de entradas, a fim de verificar se o registro atual possui todos os valores a serem buscados
        // Em seguida, para cada iteração, preciso descobrir qual o nome do campo, iterando sobre um vetor com todos os nomes dos campos
        // Após descobrir qual o nome do campo, verifico se o valor do campo do atual registro condiz com o valor do campo passado na entrada
        // Após as 'n' iterações, se todos os valores da entrada condizerem com o registro atual, então encontrado==n, e irei imprimir o registro atual

        short encontrado = 0;
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < 7; k++) {
                int num = 0;
                if (compare_strings_case_sensitive(nome_campos[j], campos_validos[k]) == 0) {
                    if (k < 3) {
                        num = atoi(valor_campos[j]);
                    }

                    switch (k) {
                        case 0:
                            encontrado += (get_id(reg) == num);
                            break;
                        case 1:
                            encontrado += (get_ano(reg) == num);
                            break;
                        case 2:
                            encontrado += (get_qtt(reg) == num);
                            break;
                        case 3:
                            encontrado += (compare_strings_case_sensitive(get_sigla(reg), valor_campos[j]) == 0);
                            break;
                        case 4:
                            encontrado += (compare_strings_case_sensitive(get_cidade(reg), valor_campos[j]) == 0);
                            break;
                        case 5:
                            encontrado += (compare_strings_case_sensitive(get_marca(reg), valor_campos[j]) == 0);
                            break;
                        case 6:
                            print_string(get_modelo(reg));
                            encontrado += (compare_strings_case_sensitive(get_modelo(reg), valor_campos[j]) == 0);
                            break;
                    }
                    break;
                }
            }
        }

        if (encontrado == n) {
            imprimir_registro(reg);
            n_registros_encontrados++;
        }

        destruir_registro(reg, 1);
    }

    if (n_registros_encontrados == 0) {
        printf("Registro inexistente\n");
    }
}


void busca_parametrizada2(FILE *arq_entrada, string_t *nome_campos, string_t *valor_campos, int n) {
    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, 2, arq_entrada);

    long long int proxByteOffset = get_proxByteOffset(cabecalho);
    
    char status = get_status(cabecalho);

    destruir_cabecalho(cabecalho);

    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");

        return;
    }

    if (proxByteOffset <= TAM_CAB_2) {
        printf("Registro inexistente\n");

        return;
    }

    long long int byteOffset_atual = TAM_CAB_2;

    string_t campos_validos[] = {"id", "ano", "qtt", "sigla", "cidade", "marca", "modelo"};
    int n_registros_encontrados = 0;
    while (byteOffset_atual < proxByteOffset) {
        registro_t *reg = criar_registro();

        int tam_reg = ler_registro(reg, 2, arq_entrada);
        byteOffset_atual += tam_reg;

        char removido = get_removido(reg);

        if (removido == '1') {
            destruir_registro(reg, 1);

            continue;
        }

        short encontrado = 0;
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < 7; k++) {
                int num = 0;
                if (compare_strings_case_sensitive(nome_campos[j], campos_validos[k]) == 0) {
                    if (k < 3) {
                        num = atoi(valor_campos[j]);
                    }

                    switch (k) {
                        case 0:
                            encontrado += (get_id(reg) == num);
                            break;
                        case 1:
                            encontrado += (get_ano(reg) == num);
                            break;
                        case 2:
                            encontrado += (get_qtt(reg) == num);
                            break;
                        case 3:
                            encontrado += (compare_strings_case_sensitive(get_sigla(reg), valor_campos[j]) == 0);
                            break;
                        case 4:
                            encontrado += (compare_strings_case_sensitive(get_cidade(reg), valor_campos[j]) == 0);
                            break;
                        case 5:
                            encontrado += (compare_strings_case_sensitive(get_marca(reg), valor_campos[j]) == 0);
                            break;
                        case 6:
                            encontrado += (compare_strings_case_sensitive(get_modelo(reg), valor_campos[j]) == 0);
                            break;
                    }
                    break;
                }
            }
        }
        if (encontrado == n) {
            imprimir_registro(reg);
            n_registros_encontrados++;
        }

        destruir_registro(reg, 1);
    }

    if (n_registros_encontrados == 0) {
        printf("Registro inexistente\n");
    }
}

void funcionalidade4(int tipo_de_arquivo, string_t binario_entrada, int RRN) {
    if (tipo_de_arquivo != 1) {
        printf("Falha no processamento do arquivo.\n");

        return;
    }

    FILE *arq_entrada = NULL;

    arq_entrada = fopen(binario_entrada, "rb");

    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");

        return;
    }

    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, 1, arq_entrada);

    int proxRRN = get_proxRRN(cabecalho);
    char status = get_status(cabecalho);

    destruir_cabecalho(cabecalho);

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

    registro_t *reg = criar_registro();

    ler_registro(reg, 1,arq_entrada);

    char removido = get_removido(reg);

    if (removido == '0') {
        imprimir_registro(reg);
    }

    destruir_registro(reg, 1);
    fclose(arq_entrada);
}