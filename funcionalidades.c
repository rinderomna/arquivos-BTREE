#include <stdio.h>
#include <stdlib.h>

#include "funcionalidades.h"
#include "cabecalhos.h"
#include "registros.h"
#include "indice.h"
#include "arvore_b.h"
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

    // Ler do cabecalho status, proxRRN e proxByteOffset
    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    char status = get_status(cabecalho);
    int proxRRN = get_proxRRN(cabecalho);
    long long int proxByteOffset = get_proxByteOffset(cabecalho);

    destruir_cabecalho(cabecalho);
    
    // Testar consistência do arquivo
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(arq_entrada);

        return;
    }

    // Para cada tipo de arquivo
    if (tipo_do_arquivo == 1) {
        // Testar se há pelo menos 1 registro
        if (proxRRN == 0) {
            printf("Registro inexistente.\n");
            fclose(arq_entrada);

            return;
        }

        // Leitura sequencial dos registros imprimindo os não removidos
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
        // Testar se há pelo menos 1 registro
        if (proxByteOffset <= TAM_CAB_2) {
            printf("Registro inexistente.\n");
            fclose(arq_entrada);

            return;
        }

        // Leitura sequencial dos registros imprimindo os não removidos
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

// Testa se um registro bate com as especificações em nome_campos e valor_campos, retornando um inteiro booleano
int registro_encontrado(registro_t *reg, string_t *nome_campos, string_t *valor_campos, int n) {
    string_t campos_validos[] = {"id", "ano", "qtt", "sigla", "cidade", "marca", "modelo"};

    if (get_removido(reg) == '1') return 0; // Se reigistro está logicamente removido, então não foi encontrado

    // 'encontrado' irá contar quantas das n condições foram atendidas pelo registro atual.
    short encontrado = 0;
    for (int condicao = 0; condicao < n; condicao++) {
        // Testar cada um dos campos válidos para saber se é um campo da condição atual
        for (int k = 0; k < 7; k++) {
            int num = 0;
            if (compare_strings_case_sensitive(nome_campos[condicao], campos_validos[k]) == 0) {
                // id, ano e qtt (k = 0, 1 ou 2) têm de ser convertidos para inteiro
                if (k < 3) {
                    num = atoi(valor_campos[condicao]);
                }

                // Se o valor do campo no registro confere com a condição, incrementar 'encontrado'
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
                        encontrado += (compare_strings_case_sensitive(get_sigla(reg), valor_campos[condicao]) == 0);
                        break;
                    case 4:
                        encontrado += (compare_strings_case_sensitive(get_cidade(reg), valor_campos[condicao]) == 0);
                        break;
                    case 5:
                        encontrado += (compare_strings_case_sensitive(get_marca(reg), valor_campos[condicao]) == 0);
                        break;
                    case 6:
                        encontrado += (compare_strings_case_sensitive(get_modelo(reg), valor_campos[condicao]) == 0);
                        break;
                }
                break;
            }
        }
    }

    // Se o todas as n especificações foram atendidas, então o registro foi encontrado
    if (encontrado == n) {
        return 1;
    }
    
    // Caso contrário, não foi encontrado
    return 0;
}

// Lê sequencialmente um arquivo de dados tipo1 e imprime os registros não removidos que batem com as especificações
void busca_parametrizada1(FILE *arq_entrada, string_t *nome_campos, string_t *valor_campos, int n) {
    // Ler do cabeçalho status e proxByteOffset
    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, 1, arq_entrada);

    int proxRRN = get_proxRRN(cabecalho);
    char status = get_status(cabecalho);

    destruir_cabecalho(cabecalho);
    
    // Testar consistência do arquivo
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");

        return;
    }

    // Testar se há pelo menos algum registro
    if (proxRRN == 0) {
        printf("Registro inexistente.\n");

        return;
    }

    int n_registros_encontrados = 0; // contador de registros que atendem especificações

    // Leitura sequencial do arquivo testando condições de busca
    for (int i = 0; i < proxRRN; i++) {
        registro_t *reg = criar_registro();

        ler_registro(reg, 1, arq_entrada);

        // Se as n condições foram atendidas, imprimir registro
        if (registro_encontrado(reg, nome_campos, valor_campos, n)) {
            imprimir_registro(reg);
            n_registros_encontrados++;
        }

        destruir_registro(reg, 1);
    }

    // Se nenhum registro atendeu às condições, informar que tal registro não existe
    if (n_registros_encontrados == 0) {
        printf("Registro inexistente.\n");
    }
}

// Lê sequencialmente um arquivo de dados tipo2 e imprime os registros não removidos que batem com as especificações
void busca_parametrizada2(FILE *arq_entrada, string_t *nome_campos, string_t *valor_campos, int n) {
    // Ler do cabeçalho status e proxByteOffset
    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, 2, arq_entrada);

    char status = get_status(cabecalho);
    long long int proxByteOffset = get_proxByteOffset(cabecalho);

    destruir_cabecalho(cabecalho);

    // Testar consistência do arquivo
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");

        return;
    }

    // Testar se há pelo menos algum registro
    if (proxByteOffset <= TAM_CAB_2) {
        printf("Registro inexistente.\n");

        return;
    }

    // Iniciar leitura após o cabeçalho
    long long int byteOffset_atual = TAM_CAB_2;

    int n_registros_encontrados = 0; // contador de registros que atendem especificações

    // Leitura sequencial do arquivo testando condições de busca
    while (byteOffset_atual < proxByteOffset) {
        // Ler próximo registro
        registro_t *reg = criar_registro();

        int tam_reg = ler_registro(reg, 2, arq_entrada);
        byteOffset_atual += tam_reg;

        if (registro_encontrado(reg, nome_campos, valor_campos, n)) {
            imprimir_registro(reg);
            n_registros_encontrados++;
        }

        destruir_registro(reg, 1);
    }

    // Se nenhum registro atendeu às condições, informar que tal registro não existe
    if (n_registros_encontrados == 0) {
        printf("Registro inexistente.\n");
    }
}

// Imprime todos os registro não removidos de um arquivo binário de tipo passado, desde que o registro 
// satisfaça 'n' condições para valores campo, condições estas lidas nas próximas n linhas da entrada padrão
void funcionalidade3(int tipo_de_arquivo, string_t binario_entrada, int n) {
    FILE *arq_entrada = fopen(binario_entrada, "rb");
    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // * nome_campos guarda os nomes dos campos que farão parte das condições de busca
    // * valor_campos guarda os valores das condições de busca em relação ao campo cujo
    // nome está na respectiva posição de nome_campos 
    string_t *nome_campos = (string_t *)malloc(n * sizeof(string_t));
    string_t *valor_campos = (string_t *)malloc(n * sizeof(string_t));

    // Ler nome_campos e valor_campos da entrada padrão
    for (int i = 0; i < n; i++) {
        nome_campos[i] = scan_quote_string();
        valor_campos[i] = scan_quote_string();
    }

    // Realiza busca parametrizada para cada respectivo tipo de arquivo
    if (tipo_de_arquivo == 1) {
        busca_parametrizada1(arq_entrada, nome_campos, valor_campos, n);
    } else if (tipo_de_arquivo == 2){
        busca_parametrizada2(arq_entrada, nome_campos, valor_campos, n);
    } else {
        printf("Tipo de arquivo não existente.\n");
    }

    fclose(arq_entrada);

    destroy_string_array(nome_campos, n);
    destroy_string_array(valor_campos, n);
}

// Imprime o registro de RRN passado de um arquivo binário, desde que ele exista e não esteja removido
void funcionalidade4(int tipo_de_arquivo, string_t binario_entrada, int RRN) {
    // Funcionalidade apenas para tipo 1 de arquivo (Registros de Tamanho Fixo)
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

    // Ler do cabeçalho status e proxRRN
    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, 1, arq_entrada);

    char status = get_status(cabecalho);
    int proxRRN = get_proxRRN(cabecalho);

    destruir_cabecalho(cabecalho);

    // Testar consistência do arquivo
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");

        fclose(arq_entrada);
        
        return;
    }

    // Testar se RRN é válido
    if (RRN >= proxRRN || RRN < 0) {
        printf("Registro inexistente.\n");

        fclose(arq_entrada);
        
        return;
    }
    
    // Calcular Byte Offset para RRN passado
    long long int byte_offset = TAM_CAB_1 + RRN * TAM_REG1;

    // Pular para Byte Offset calculado
    fseek(arq_entrada, byte_offset, SEEK_SET);

    // Ler e imprimir registro se ele não estiver removido
    registro_t *reg = criar_registro();

    ler_registro(reg, 1,arq_entrada);

    char removido = get_removido(reg);
    
    if (removido == '0') {
        imprimir_registro(reg);
    } else {
        printf("Registro inexistente.\n");
    }

    destruir_registro(reg, 1);
    fclose(arq_entrada);
}

// Cria um arquivo de índice primário a partir de um arquivo de dados existente de tipo indicado
// A chave primária é o campo id do registros de dados
// Para o tipo1, guarda-se para cada chave primária o RRN do registro no arquivo de dados
// Para o tipo2, guarda-se para cada chave primária o Byteoffset do registro no arquivo de dados
void funcionalidade5(int tipo_do_arquivo, string_t arquivo_de_dados, string_t arquivo_de_indice) {
    FILE *arq_entrada = NULL;

    arq_entrada = fopen(arquivo_de_dados, "rb");

    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");

        return;
    }
    
    // Ler do cabecalho status, proxRRN e proxByteOffset
    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    char status = get_status(cabecalho);
    int proxRRN = get_proxRRN(cabecalho);
    long long int proxByteOffset = get_proxByteOffset(cabecalho);

    destruir_cabecalho(cabecalho);
    
    // Testar consistência do arquivo
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(arq_entrada);

        return;
    }

    //---------------------------------------------------------------------------------

    // Criando índice em RAM
    indice_t *indice = criar_indice();

    // Ler sequencialmente o arquivo de dados salvando as informações no índice em RAM:

    if (tipo_do_arquivo == 1) {
        // Leitura sequencial dos registros adicionando no índice as informações dos não removidos:
        for (int rrn = 0; rrn < proxRRN; rrn++) {
            registro_t *reg = criar_registro();

            ler_registro(reg, tipo_do_arquivo, arq_entrada);
            
            char removido = get_removido(reg);

            if (removido == '0') {
                registro_de_indice_t *ri = criar_registro_indice();
                set_id_registro_indice(ri, get_id(reg));
                set_rrn_registro_indice(ri, rrn);
                adicionar_registro_a_indice(indice, ri);
            }

            destruir_registro(reg, 1);
        }
    } else if (tipo_do_arquivo == 2) {
        // Leitura sequencial dos registros adicionando no índice as informações dos não removidos:
        long long int byteOffset_atual = TAM_CAB_2;
        while (byteOffset_atual < proxByteOffset) {
            registro_t *reg = criar_registro();

            int tam_reg = ler_registro(reg, tipo_do_arquivo, arq_entrada);

            char removido = get_removido(reg);

            if (removido == '0') {
                registro_de_indice_t *ri = criar_registro_indice();
                set_id_registro_indice(ri, get_id(reg));
                set_byte_offset_registro_indice(ri, byteOffset_atual);
                adicionar_registro_a_indice(indice, ri);
            }

            byteOffset_atual += tam_reg;
            destruir_registro(reg, 1);
        }
    }    

    // Escrever o arquivo de índice atualizado
    escrever_indice(indice, tipo_do_arquivo, arquivo_de_indice);
    destruir_indice(indice);
    fclose(arq_entrada);

    // Imprimir "identificação" do arquivo de índice
    binarioNaTela(arquivo_de_indice);
}

// Remove registro do arquivo de dados e do índice em RAM correspondente
// Enviar cabecalho correspondente
// O status no arquivo de dados deve ser antes setado para '0'
// Apenas enviar posição de remoção que já foi checada ser válida e registro não esteja removido
void remover_do_arquivo_por_posicao(int tipo_do_arquivo, FILE *arq_entrada, cabecalho_t *cabecalho, indice_t *indice, long long int posicao_de_remocao) {
    // Ler do cabecalho nroRegRem e topo do cabecalho 

    int nroRegRem = get_nroRegRem(cabecalho);
    long long int topo = get_topo(cabecalho);

    set_nroRegRem(cabecalho, nroRegRem + 1); // Incrementar no cabecalho número de registros removidos

    if (tipo_do_arquivo == 1) { // pilha de removidos
        int RRN_a_remover = (int)posicao_de_remocao;

        // Calcular Byte Offset para RRN passado
        long long int byte_offset_a_remover = TAM_CAB_1 + RRN_a_remover * TAM_REG1;

        registro_t *reg = criar_registro();

        // Ler registro a ser removido em sua posição
        fseek(arq_entrada, byte_offset_a_remover, SEEK_SET);
        ler_registro(reg, tipo_do_arquivo, arq_entrada);
        set_removido(reg, '1'); // Marcar como logicamente removido
        remover_do_indice_por_id(tipo_do_arquivo, indice, get_id(reg)); // Remover do arquivo de índice

        set_proxRRN_removido(reg, (int)topo); // Faz o registro sendo removido apontar para o topo anterior
        set_topo(cabecalho, posicao_de_remocao); // Coloca o registro sendo removido no topo
        
        // Retorna posição do ponteiro de arquivo para o início do registro
        fseek(arq_entrada, byte_offset_a_remover, SEEK_SET);
        
        // Escrever registro (agora logicamente removido) de volta no arquivo
        escrever_registro1_em_arquivo(reg, arq_entrada);

        destruir_registro(reg, 1);
    } else if (tipo_do_arquivo == 2) { // Lista decrescente de registros removidos
        long long int byte_offset_a_remover = posicao_de_remocao;

        registro_t *reg = criar_registro();
        fseek(arq_entrada, byte_offset_a_remover, SEEK_SET);
        int tam_reg = ler_registro(reg, tipo_do_arquivo, arq_entrada);
        set_removido(reg, '1'); // Marcar como logicamente removido

        remover_do_indice_por_id(tipo_do_arquivo, indice, get_id(reg)); // Remover do arquivo de índice

        // Encontrando posição em que o registro será introduzido na lista ordenada de removidos:
        long long int posicao_do_anterior = -1; 
        long long int posicao_do_atual = topo;
        int tam_reg_comparado = -1;
        
        while (1) {
            // Parar a busca se a posição do próximo é "nula" (-1)
            if (posicao_do_atual == -1) break; 

            // Posicionar no inicio do campo de tamanho de registro a comparar (-5)
            fseek(arq_entrada, posicao_do_atual + 1, SEEK_SET);

            // Ler tamanho do registro a ser comparado para decidir
            fread(&tam_reg_comparado, sizeof(int), 1, arq_entrada);
            tam_reg_comparado += 5; // Considerando campos iniciais no tamanho do registro
            
            // Parar também se o próximo do encadeamento tem tamanho menor ou igual ao registro que está sendo removido
            if (tam_reg >= tam_reg_comparado) break; 

            // Como seguiremos, o atual será o anterior
            posicao_do_anterior = posicao_do_atual;
    
            // Ler o próximo removido para pular
            fread(&posicao_do_atual, sizeof(long long int), 1, arq_entrada);
        }
        
        if (posicao_do_anterior == -1) { // inserção no início (Não há anterior no encadeamento)
            set_proxByteOffset_removido(reg, topo); // Encadear topo anterior como próximo removido
            set_topo(cabecalho, byte_offset_a_remover); // Atualizar topo com a posição do registro sendo removido
        } else {
            // Posicionar no início do campo de prox removido do registro removido anterior
            fseek(arq_entrada, posicao_do_anterior + 5, SEEK_SET);
            
            // Encadear registro sendo removido como próximo do anterior
            fwrite(&byte_offset_a_remover, sizeof(long long int), 1, arq_entrada);

            // Encadear registro comparado (menor ou igual ao sendo removido) como próximo do removido
            set_proxByteOffset_removido(reg, posicao_do_atual);
        }

        // Retorna posição do ponteiro de arquivo para o início do registro
        fseek(arq_entrada, byte_offset_a_remover, SEEK_SET);

        // Escrever registro no arquivo de dados
        escrever_registro2_em_arquivo(reg, arq_entrada);

        destruir_registro(reg, 1);
    }
}

// Remove do arquivo de dados e índice em RAM correspondente todos os registros que batem com as especificações em nome_campos e valor_campos
// nome_campos contém os nomes dos campos cujos valores de especificação encontram-se em cada respectiva posição de valor_campos
// A busca por tais registros é feita de forma sequencial no arquivo de dados
void remover_sequencialmente(int tipo_do_arquivo, FILE *arq_entrada, cabecalho_t *cabecalho, indice_t *indice, string_t *nome_campos, string_t *valor_campos, int n_campos) {
    int proxRRN = get_proxRRN(cabecalho);
    long long int proxByteOffset = get_proxByteOffset(cabecalho);

    // Para cada tipo de arquivo
    if (tipo_do_arquivo == 1) {
        // Testar se há pelo menos 1 registro
        if (proxRRN == 0) {
            fclose(arq_entrada);

            return;
        }

        // Leitura sequencial dos registros removendo os que batem e não são removidos

        // Posicionando o ponteiro do arquivo logo após o cabeçalho
        fseek(arq_entrada, TAM_CAB_1, SEEK_SET);

        for (int RRN_atual = 0; RRN_atual < proxRRN; RRN_atual++) {
            registro_t *reg = criar_registro();
            ler_registro(reg, tipo_do_arquivo, arq_entrada);

            if (registro_encontrado(reg, nome_campos, valor_campos, n_campos)) {           
                remover_do_arquivo_por_posicao(tipo_do_arquivo, arq_entrada, cabecalho, indice, RRN_atual);
            }

            destruir_registro(reg, 1);
        }
    } else if (tipo_do_arquivo == 2) {
        // Testar se há pelo menos 1 registro
        if (proxByteOffset <= TAM_CAB_2) {
            fclose(arq_entrada);

            return;
        }

        // Leitura sequencial dos registros imprimindo os não removidos

        // Posicionando o ponteiro do arquivo logo após o cabeçalho
        fseek(arq_entrada, TAM_CAB_2, SEEK_SET);

        long long int byteOffset_atual = TAM_CAB_2;

        while (byteOffset_atual < proxByteOffset) {
            registro_t *reg = criar_registro();

            int tam_reg = ler_registro(reg, tipo_do_arquivo, arq_entrada);

            if (registro_encontrado(reg, nome_campos, valor_campos, n_campos)) {
                remover_do_arquivo_por_posicao(tipo_do_arquivo, arq_entrada, cabecalho, indice, byteOffset_atual);
            }

            byteOffset_atual += tam_reg;

            destruir_registro(reg, 1);
        }
    }
}


// Retorna o índice em que o campo id está no vetor de campos ou -1 caso não haja este campo
int tem_id_onde(string_t *campos, int n_campos) {
    for (int i = 0; i < n_campos; i++) {
        if (compare_strings_case_sensitive(campos[i], "id") == 0) {
            return i;
        }
    }

    return -1;
} 

// Remove do arquivo de dados e de índice correspondente 'n' registros especificados na entrada padrão
void funcionalidade6(int tipo_do_arquivo, string_t binario_entrada, string_t arquivo_de_indice, int n) {
    // Abrir arquivo para leitura e escrita de binário
    FILE *arq_entrada = fopen(binario_entrada, "r+b");
    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler cabecalho e testar consistência
    cabecalho_t *cabecalho = criar_cabecalho();
    ler_cabecalho_de_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    char status = get_status(cabecalho);
    
    // Testar consistência do arquivo
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");
        destruir_cabecalho(cabecalho);
        fclose(arq_entrada);

        return;
    }

    // Arquivo potencialmente inconsistente neste ponto, escrever status '0'
    set_status(cabecalho, '0');
    escrever_cabecalho_em_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);

    // Carregar índice para RAM
    indice_t *indice = ler_indice(tipo_do_arquivo, arquivo_de_indice);

    if (!indice) {
        destruir_cabecalho(cabecalho);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Laço que realiza cada uma das n remoções, lendo especificações de busca da entrada padrão
    for (int remocao = 0; remocao < n; remocao++) {
        // Ler quantos campos serão utilizados para especificar a remoção
        int n_campos = 0;
        scanf(" %d", &n_campos);

        // Ler nome_campos e valor_campos da entrada padrão
        // * nome_campos guarda os nomes dos campos que farão parte das condições de busca
        // * valor_campos guarda os valores das condições de busca em relação ao campo cujo
        //     nome está na respectiva posição de nome_campos 
        string_t *nome_campos = (string_t *)malloc(n_campos * sizeof(string_t));
        string_t *valor_campos = (string_t *)malloc(n_campos * sizeof(string_t));

        for (int i = 0; i < n_campos; i++) {
            nome_campos[i] = scan_quote_string();
            valor_campos[i] = scan_quote_string();
        }

        int id_pos = tem_id_onde(nome_campos, n_campos);

        if (id_pos != -1) { // Remover buscando pelo índice primário
            int id_a_remover = atoi(valor_campos[id_pos]);

            long long int posicao_de_remocao = busca_no_indice(indice, tipo_do_arquivo, id_a_remover, NULL);

            if (posicao_de_remocao == -1) { // id não existe
                destroy_string_array(nome_campos, n_campos);
                destroy_string_array(valor_campos, n_campos);
                continue; 
            }

            if (n_campos == 1) { // Se o campo for apenas id, remover na posição lida do índice
                remover_do_arquivo_por_posicao(tipo_do_arquivo, arq_entrada, cabecalho, indice, posicao_de_remocao);                
            } else { // Se houverem mais campos de especificações, testá-los no registro buscado antes de remover
                registro_t *reg = get_registro_em_posicao(tipo_do_arquivo, posicao_de_remocao, arq_entrada);

                if (registro_encontrado(reg, nome_campos, valor_campos, n_campos)) {
                    remover_do_arquivo_por_posicao(tipo_do_arquivo, arq_entrada, cabecalho, indice, posicao_de_remocao);                
                }

                destruir_registro(reg, 1);
            }
        } else { // Leitura sequencial do arquivo removendo caso registro bata com as especificações
            remover_sequencialmente(tipo_do_arquivo, arq_entrada, cabecalho, indice, nome_campos, valor_campos, n_campos);
        }

        destroy_string_array(nome_campos, n_campos);
        destroy_string_array(valor_campos, n_campos);
    }

    // Arquivo consistente após todas as alterações realizadas:
    set_status(cabecalho, '1'); 
    escrever_cabecalho_em_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    destruir_cabecalho(cabecalho);

    // Escrever índice em RAM em arquivo
    escrever_indice(indice, tipo_do_arquivo, arquivo_de_indice);
    destruir_indice(indice);

    fclose(arq_entrada);

    // Imprimir binário na tela de Arquivo de Dados e Arquivo de Índice, respectivamente
    binarioNaTela(binario_entrada);
    binarioNaTela(arquivo_de_indice);
}

// Preenche as informações de um registro com as informações no vetor de strings campos, de 7 posições,
// 1 para cada campo respectivo: id, ano de fabricação, quantidade, sigla, cidade, marca, modelo
void preencher_registro(registro_t *reg, string_t *campos) {
    // id (campo obrigatório)
    set_id(reg, atoi(campos[0]));

    // Ano de fabricação
    if (campos[1] && string_length(campos[1]) > 0) {
        set_ano(reg, atoi(campos[1]));
    } else {
        set_ano(reg, -1);
    }

    // Quantidade
    if (campos[2] && string_length(campos[2]) > 0) {
        set_qtt(reg, atoi(campos[2]));
    } else {
        set_qtt(reg, -1);
    }

    set_sigla(reg, campos[3]);
    set_cidade(reg, campos[4]);
    set_marca(reg, campos[5]);
    set_modelo(reg, campos[6]);
}

// Insere registro em arquivo de dados e índice em RAM correspondente, seguindo a devida lógica de reaproveitamento de espaço:
// Tipo 1 -> Desempilha o último removido para reaproveitar, ou insere no final se não houver removidos
// Tipo 2 -> Aplica Worst Fit para reaproveitar espaço, ou insere no final se não houver removidos ou se não couber em nenhum deles
void inserir_registro(int tipo_do_arquivo, FILE *arq_entrada, cabecalho_t *cabecalho, indice_t *indice, registro_t *registro_a_adicionar) {
    // Criar registro de índice que será adicionado ao índice
    registro_de_indice_t *ri = criar_registro_indice();

    // Setando o id deste registro de índice
    set_id_registro_indice(ri, get_id(registro_a_adicionar));

    // Ler topo do cabecalho
    long long int topo = get_topo(cabecalho);

    if (tipo_do_arquivo == 1) { // desempilhar ou final
        if (topo == -1) { 
            // Inserir no final do arquivo
            fseek(arq_entrada, 0, SEEK_END);
            escrever_registro1_em_arquivo(registro_a_adicionar, arq_entrada);

            // Setar este RRN no registro de índice
            int proxRRN = get_proxRRN(cabecalho);
            set_rrn_registro_indice(ri, proxRRN);

            // Alterar proxRRN no cabecalho
            set_proxRRN(cabecalho, proxRRN + 1);
        } else {
            // Buscando o registro que será reutilizado no topo anterior
            registro_t *registro_reutilizado = get_registro_em_posicao(tipo_do_arquivo, topo, arq_entrada);

            // Atualizando topo para o próximo do registro que será reutilizado
            set_topo(cabecalho, get_proxRRN_removido(registro_reutilizado));
            destruir_registro(registro_reutilizado, 1);

            // Escrever na posição do registro que está sendo reutilizado (topo anterior)
            escrever_registro_em_posicao(tipo_do_arquivo, registro_a_adicionar, topo, arq_entrada); 

            // Setar esta posicao no registro de índice (topo anterior)
            set_rrn_registro_indice(ri, topo);

            // Diminuir número de registros removidos
            int nroRegRem = get_nroRegRem(cabecalho);
            set_nroRegRem(cabecalho, nroRegRem - 1);
        }
    } else if (tipo_do_arquivo == 2) { // tirar da lista ou final
        if (topo == -1) {
            // Inserir no final do arquivo
            fseek(arq_entrada, 0, SEEK_END);
            int tam_reg = escrever_registro2_em_arquivo(registro_a_adicionar, arq_entrada);

            // Setar este byteoffset no registro de índice
            int proxByteOffset = get_proxByteOffset(cabecalho);
            set_byte_offset_registro_indice(ri, proxByteOffset);

            // Alterar proxRRN no cabecalho
            set_proxByteOffset(cabecalho, proxByteOffset + tam_reg);
        } else { // Worst Fit
            // Buscar o maior registro reutilizável no topo da lista ordenada
            registro_t *maior_registro_reutilizavel = get_registro_em_posicao(tipo_do_arquivo, topo, arq_entrada);
            
            // Ler o tamanho deste maior reutilizável 
            int tam_maior_reutilizavel = 0;
            fseek(arq_entrada, topo + 1, SEEK_SET);
            fread(&tam_maior_reutilizavel, sizeof(int), 1, arq_entrada);
            tam_maior_reutilizavel += 5; // Incrementar 5 para contar os bytes iniciais

            // Computar tamanho que o registro que será adicionado irá precisar
            int tam_reg_a_adicionar = tamanho_total_do_registro(tipo_do_arquivo, registro_a_adicionar);

            // Testar se o registro a ser adicionado cabe no maior espaço reutilizável
            if (tam_reg_a_adicionar <= tam_maior_reutilizavel) { // Caso caiba, reaproveitar
                // Posicionar ponteiro do arquivo onde irá reutilizar
                fseek(arq_entrada, topo, SEEK_SET);

                // Escrever em cima o registro sendo utilizado
                reescrever_registro2_em_arquivo(registro_a_adicionar, tam_maior_reutilizavel, arq_entrada);

                // Atualizar o topo para o próximo do registro que está sendo reutilizado
                set_topo(cabecalho, get_proxByteOffset_removido(maior_registro_reutilizavel));

                // Setar byteoffset do registro que está sendo reutilizado no registro de índice
                set_byte_offset_registro_indice(ri, topo);

                // Diminuir número de registros removidos
                int nroRegRem = get_nroRegRem(cabecalho);
                set_nroRegRem(cabecalho, nroRegRem - 1);
            } else { // Não cabe -> Inserir no final
                // Posicionar ponteiro no final do arquivo e escrever registro
                fseek(arq_entrada, 0, SEEK_END);
                int tam_reg = escrever_registro2_em_arquivo(registro_a_adicionar, arq_entrada);

                // Setar este byteoffset no registro de índice
                int proxByteOffset = get_proxByteOffset(cabecalho);
                set_byte_offset_registro_indice(ri, proxByteOffset);    

                // Alterar proxRRN no cabecalho
                set_proxByteOffset(cabecalho, proxByteOffset + tam_reg);
            }

            destruir_registro(maior_registro_reutilizavel, 1);
        }
    }

    // Adicionar novo registro a índice em RAM
    adicionar_registro_a_indice(indice, ri);
}

// Insere registro em arquivo de dados e índice árvore-B correspondente, seguindo a devida lógica de reaproveitamento de espaço:
// Tipo 1 -> Desempilha o último removido para reaproveitar, ou insere no final se não houver removidos
// Tipo 2 -> Aplica Worst Fit para reaproveitar espaço, ou insere no final se não houver removidos ou se não couber em nenhum deles
// Obs.: Arquivo de árvore-B deve já estar aberto para leitura e escrita
void inserir_registro_arvore(int tipo_do_arquivo, FILE *arq_entrada, cabecalho_t *cabecalho, FILE *arq_arvore, cabecalho_arvore_t *cab_arvore, registro_t *registro_a_adicionar) {
    // Criar chave que será adicionada ao índice árvore-B já setando id:
    chave_t chave_a_adicionar = {.id = get_id(registro_a_adicionar)};

    // Ler topo do cabecalho
    long long int topo = get_topo(cabecalho);

    if (tipo_do_arquivo == 1) { // desempilhar ou final
        if (topo == -1) { 
            // Inserir no final do arquivo
            fseek(arq_entrada, 0, SEEK_END);
            escrever_registro1_em_arquivo(registro_a_adicionar, arq_entrada);

            // Setar este RRN na chave que será adicionada ao índice
            int proxRRN = get_proxRRN(cabecalho);
            chave_a_adicionar.RRN_dados = proxRRN;

            // Alterar proxRRN no cabecalho
            set_proxRRN(cabecalho, proxRRN + 1);
        } else {
            // Buscando o registro que será reutilizado no topo anterior
            registro_t *registro_reutilizado = get_registro_em_posicao(tipo_do_arquivo, topo, arq_entrada);

            // Atualizando topo para o próximo do registro que será reutilizado
            set_topo(cabecalho, get_proxRRN_removido(registro_reutilizado));
            destruir_registro(registro_reutilizado, 1);

            // Escrever na posição do registro que está sendo reutilizado (topo anterior)
            escrever_registro_em_posicao(tipo_do_arquivo, registro_a_adicionar, topo, arq_entrada); 

            // Setar esta posicao na chave que será adicionada ao índice (topo anterior)
            chave_a_adicionar.RRN_dados = (int)topo;

            // Diminuir número de registros removidos
            int nroRegRem = get_nroRegRem(cabecalho);
            set_nroRegRem(cabecalho, nroRegRem - 1);
        }
    } else if (tipo_do_arquivo == 2) { // tirar da lista ou final
        if (topo == -1) {
            // Inserir no final do arquivo
            fseek(arq_entrada, 0, SEEK_END);
            int tam_reg = escrever_registro2_em_arquivo(registro_a_adicionar, arq_entrada);

            // Setar este byteoffset na chave que será adicionada ao índice
            int proxByteOffset = get_proxByteOffset(cabecalho);
            chave_a_adicionar.byteoffset_dados = proxByteOffset;

            // Alterar proxRRN no cabecalho
            set_proxByteOffset(cabecalho, proxByteOffset + tam_reg);
        } else { // Worst Fit
            // Buscar o maior registro reutilizável no topo da lista ordenada
            registro_t *maior_registro_reutilizavel = get_registro_em_posicao(tipo_do_arquivo, topo, arq_entrada);
            
            // Ler o tamanho deste maior reutilizável 
            int tam_maior_reutilizavel = 0;
            fseek(arq_entrada, topo + 1, SEEK_SET);
            fread(&tam_maior_reutilizavel, sizeof(int), 1, arq_entrada);
            tam_maior_reutilizavel += 5; // Incrementar 5 para contar os bytes iniciais

            // Computar tamanho que o registro que será adicionado irá precisar
            int tam_reg_a_adicionar = tamanho_total_do_registro(tipo_do_arquivo, registro_a_adicionar);

            // Testar se o registro a ser adicionado cabe no maior espaço reutilizável
            if (tam_reg_a_adicionar <= tam_maior_reutilizavel) { // Caso caiba, reaproveitar
                // Posicionar ponteiro do arquivo onde irá reutilizar
                fseek(arq_entrada, topo, SEEK_SET);

                // Escrever em cima o registro sendo utilizado
                reescrever_registro2_em_arquivo(registro_a_adicionar, tam_maior_reutilizavel, arq_entrada);

                // Atualizar o topo para o próximo do registro que está sendo reutilizado
                set_topo(cabecalho, get_proxByteOffset_removido(maior_registro_reutilizavel));

                // Setar byteoffset do registro que está sendo reutilizado na chave de índice
                chave_a_adicionar.byteoffset_dados = topo;

                // Diminuir número de registros removidos
                int nroRegRem = get_nroRegRem(cabecalho);
                set_nroRegRem(cabecalho, nroRegRem - 1);
            } else { // Não cabe -> Inserir no final
                // Posicionar ponteiro no final do arquivo e escrever registro
                fseek(arq_entrada, 0, SEEK_END);
                int tam_reg = escrever_registro2_em_arquivo(registro_a_adicionar, arq_entrada);

                // Setar este byteoffset na chave que será adionada ao índice
                int proxByteOffset = get_proxByteOffset(cabecalho);
                chave_a_adicionar.byteoffset_dados = proxByteOffset;  

                // Alterar proxRRN no cabecalho
                set_proxByteOffset(cabecalho, proxByteOffset + tam_reg);
            }

            destruir_registro(maior_registro_reutilizavel, 1);
        }
    }

    inserir_chave_em_arvore(&chave_a_adicionar, tipo_do_arquivo, cab_arvore, arq_arvore);
}

// Insere no arquivo de dados e de índice correspondente 'n' novos registro lidos entrada padrão
void funcionalidade7(int tipo_do_arquivo, string_t binario_entrada, string_t arquivo_de_indice, int n_insercoes) {
    // Abrir arquivo para leitura e escrita de binário
    FILE *arq_entrada = fopen(binario_entrada, "r+b");
    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler cabecalho e testar consistência
    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    char status = get_status(cabecalho);
    
    // Testar consistência do arquivo
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");
        destruir_cabecalho(cabecalho);
        fclose(arq_entrada);

        return;
    }

    // Arquivo potencialmente inconsistente neste ponto
    set_status(cabecalho, '0');
    escrever_cabecalho_em_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);

    // Carregar índice para RAM
    indice_t *indice = ler_indice(tipo_do_arquivo, arquivo_de_indice);

    if (!indice) {
        destruir_cabecalho(cabecalho);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Laço que realiza cada uma das inserções especificadas na entrada padrão
    for (int insercao = 0; insercao < n_insercoes; insercao++) {
        // Ler campos de novo registro da entrada padrão
        string_t *campos = (string_t *)malloc(7 * sizeof(string_t));

        for (int i = 0; i < 7; i++) {
            campos[i] = scan_quote_string();
        }

        // Cria preenche e insere registro no arquivo de dados e índice em RAM
        registro_t *registro_a_adicionar = criar_registro();
        preencher_registro(registro_a_adicionar, campos);
        inserir_registro(tipo_do_arquivo, arq_entrada, cabecalho, indice, registro_a_adicionar);

        destroy_string_array(campos, 7);
        destruir_registro(registro_a_adicionar, 0);
    }

    // Arquivo consistente após todas as alterações realizadas:
    set_status(cabecalho, '1'); 
    escrever_cabecalho_em_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    destruir_cabecalho(cabecalho);

    // Escrever índice em RAM em arquivo
    escrever_indice(indice, tipo_do_arquivo, arquivo_de_indice);
    destruir_indice(indice);

    fclose(arq_entrada);

    // Imprimir binário na tela de Arquivo de Dados e Arquivo de Índice, respectivamente
    binarioNaTela(binario_entrada);
    binarioNaTela(arquivo_de_indice);
}

// Altera as informações de um registro para valores especificados em nome_campos_alteracao e valor_campos_alteração
void alterar_registro(registro_t *reg, string_t *nome_campos_alteracao, string_t *valor_campos_alteracao, int n_campos_alteracao) {
    string_t campos_validos[] = {"id", "ano", "qtt", "sigla", "cidade", "marca", "modelo"};

    for (int alteracao = 0; alteracao < n_campos_alteracao; alteracao++) {
        // Testar cada um dos campos válidos para saber se é um campo da especificação de alteração
        for (int k = 0; k < 7; k++) {
            int num = 0;
            if (compare_strings_case_sensitive(nome_campos_alteracao[alteracao], campos_validos[k]) == 0) {
                // id, ano e qtt (k = 0, 1 ou 2) têm de ser convertidos para inteiro
                if (k < 3) {
                    if (string_length(valor_campos_alteracao[alteracao]) > 0) {
                        num = atoi(valor_campos_alteracao[alteracao]);
                    } else {
                        num = -1;
                    }
                }

                switch (k) {
                    case 0:
                        set_id(reg, num);
                        break;
                    case 1:
                        set_ano(reg, num);
                        break;
                    case 2:
                        set_qtt(reg, num);
                        break;
                    case 3:
                        if (get_sigla(reg)) free(get_sigla(reg));
                        set_sigla(reg, clone_string(valor_campos_alteracao[alteracao]));
                        break;
                    case 4:
                        if (get_cidade(reg)) free(get_cidade(reg));
                        set_cidade(reg, clone_string(valor_campos_alteracao[alteracao]));
                        break;
                    case 5:
                        if (get_marca(reg)) free(get_marca(reg));
                        set_marca(reg, clone_string(valor_campos_alteracao[alteracao]));
                        break;
                    case 6:
                        if (get_modelo(reg)) free(get_modelo(reg));
                        set_modelo(reg, clone_string(valor_campos_alteracao[alteracao]));
                        break;
                }
                break;
            }
        }
    }


}

// Alterar registro do arquivo de dados e do índice em RAM correspondente
// Enviar cabecalho correspondente
// O status no arquivo de dados deve ser antes setado para '0'
// Apenas enviar posição de alteração que já foi checada ser válida e registro não esteja removido
void alterar_do_arquivo_por_posicao(int tipo_do_arquivo, FILE *arq_entrada, cabecalho_t *cabecalho, indice_t *indice, long long int posicao_de_alteracao, string_t *nome_campos_alteracao, string_t *valor_campos_alteracao, int n_campos_alteracao) {
    if (tipo_do_arquivo == 1) { 
        int RRN_a_alterar = (int)posicao_de_alteracao;

        // Calcular Byte Offset para RRN passado
        long long int byte_offset_a_alterar = TAM_CAB_1 + RRN_a_alterar * TAM_REG1;

        registro_t *reg = criar_registro();

        fseek(arq_entrada, byte_offset_a_alterar, SEEK_SET);
        ler_registro(reg, tipo_do_arquivo, arq_entrada);
        
        // Caso for alterar id para novo id, atualizar no indice
        int id_pos = tem_id_onde(nome_campos_alteracao, n_campos_alteracao);

        if (id_pos != -1) {
            int id_original = get_id(reg);
            int novo_id = atoi(valor_campos_alteracao[id_pos]);

            remover_do_indice_por_id(tipo_do_arquivo, indice, id_original);

            registro_de_indice_t *ri = criar_registro_indice();
            set_id_registro_indice(ri, novo_id);
            set_rrn_registro_indice(ri, RRN_a_alterar);

            adicionar_registro_a_indice(indice, ri);
        }

        alterar_registro(reg, nome_campos_alteracao, valor_campos_alteracao, n_campos_alteracao);
        
        // Retorna posição do ponteiro de arquivo para o início do registro
        fseek(arq_entrada, byte_offset_a_alterar, SEEK_SET);        
        escrever_registro1_em_arquivo(reg, arq_entrada);
        destruir_registro(reg, 1);
    } else if (tipo_do_arquivo == 2) { // Lista decrescente de registros removidos
        long long int byte_offset_a_alterar = posicao_de_alteracao;

        registro_t *reg = criar_registro();
        fseek(arq_entrada, byte_offset_a_alterar, SEEK_SET);
        
        int tam_reg = ler_registro(reg, tipo_do_arquivo, arq_entrada);

        alterar_registro(reg, nome_campos_alteracao, valor_campos_alteracao, n_campos_alteracao);

        int novo_tam_reg = tamanho_total_do_registro(tipo_do_arquivo, reg);

        if (novo_tam_reg <= tam_reg) {
            // Caso for alterar id para novo id, atualizar no indice        
            int id_pos = tem_id_onde(nome_campos_alteracao, n_campos_alteracao);

            if (id_pos != -1) {
                int id_original = get_id(reg);
                int novo_id = atoi(valor_campos_alteracao[id_pos]);

                remover_do_indice_por_id(tipo_do_arquivo, indice, id_original);

                registro_de_indice_t *ri = criar_registro_indice();
                set_id_registro_indice(ri, novo_id);
                set_byte_offset_registro_indice(ri, byte_offset_a_alterar);

                adicionar_registro_a_indice(indice, ri);
            }

            fseek(arq_entrada, byte_offset_a_alterar, SEEK_SET);
            reescrever_registro2_em_arquivo(reg, tam_reg, arq_entrada);
        } else {
            remover_do_arquivo_por_posicao(tipo_do_arquivo, arq_entrada, cabecalho, indice, byte_offset_a_alterar);
            inserir_registro(tipo_do_arquivo, arq_entrada, cabecalho, indice, reg);
        }

        destruir_registro(reg, 1);
    }
}

// Altera do arquivo de dados e e índice  em RAM correspondente todos os registros que batem com as especificações em nome_campos_busca e valor_campos_busca,
// para novas especificações em nome_campos_alteracao e valores_campos_alteracao.
// A busca por tais registros é feita de forma sequencial no arquivo de dados
void alterar_sequencialmente(int tipo_do_arquivo, FILE *arq_entrada, cabecalho_t *cabecalho, indice_t *indice, string_t *nome_campos_busca, string_t *valor_campos_busca, int n_campos_busca, string_t *nome_campos_alteracao, string_t *valor_campos_alteracao, int n_campos_alteracao) {
    int proxRRN = get_proxRRN(cabecalho);
    long long int proxByteOffset = get_proxByteOffset(cabecalho);

    // Para cada tipo de arquivo, leitura sequencial do arquivo, alterando os que baterem com as especificações
    if (tipo_do_arquivo == 1) {
        // Posicionando o ponteiro do arquivo logo após o cabeçalho
        fseek(arq_entrada, TAM_CAB_1, SEEK_SET);

        for (int RRN_atual = 0; RRN_atual < proxRRN; RRN_atual++) {
            registro_t *reg = criar_registro();

            ler_registro(reg, tipo_do_arquivo, arq_entrada);

            if (registro_encontrado(reg, nome_campos_busca, valor_campos_busca, n_campos_busca)) {                 
                alterar_do_arquivo_por_posicao(tipo_do_arquivo, arq_entrada, cabecalho, indice, RRN_atual, nome_campos_alteracao, valor_campos_alteracao, n_campos_alteracao);
                fseek(arq_entrada, TAM_CAB_1 + (RRN_atual + 1) * TAM_REG1, SEEK_SET); // Reposicionando ponteiro para próxima leitura
            }

            destruir_registro(reg, 1);
        }
    } else if (tipo_do_arquivo == 2) {
        // Posicionando o ponteiro do arquivo logo após o cabeçalho
        fseek(arq_entrada, TAM_CAB_2, SEEK_SET);

        long long int byteOffset_atual = TAM_CAB_2;

        while (byteOffset_atual < proxByteOffset) {
            registro_t *reg = criar_registro();

            int tam_reg = ler_registro(reg, tipo_do_arquivo, arq_entrada);

            if (registro_encontrado(reg, nome_campos_busca, valor_campos_busca, n_campos_busca)) {
                alterar_do_arquivo_por_posicao(tipo_do_arquivo, arq_entrada, cabecalho, indice, byteOffset_atual, nome_campos_alteracao, valor_campos_alteracao, n_campos_alteracao);
                fseek(arq_entrada, byteOffset_atual + tam_reg, SEEK_SET); // Reposicionando ponteiro para próxima leitura
            }

            byteOffset_atual += tam_reg;

            destruir_registro(reg, 1);
        }
    }
}

// Altera no arquivo de dados e de índice correspondente 'n_alteracoes' registro lidos especificados na entrada padrão.
void funcionalidade8(int tipo_do_arquivo, string_t binario_entrada, string_t arquivo_de_indice, int n_alteracoes) {
    // Abrir arquivo para leitura e escrita de binário
    FILE *arq_entrada = fopen(binario_entrada, "r+b");
    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler cabecalho e testar consistência
    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    char status = get_status(cabecalho);
    
    // Testar consistência do arquivo
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");
        destruir_cabecalho(cabecalho);
        fclose(arq_entrada);

        return;
    }

    // Arquivo potencialmente inconsistente neste ponto
    set_status(cabecalho, '0');
    escrever_cabecalho_em_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);

    // Carregar índice para RAM
    indice_t *indice = ler_indice(tipo_do_arquivo, arquivo_de_indice);

    if (!indice) {
        destruir_cabecalho(cabecalho);
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Laço que realiza cada uma das alterações especificadas na entrada padrão
    for (int alteracoes = 0; alteracoes < n_alteracoes; alteracoes++) {
        // Ler quantos campos de busca serão usados nessa alteração
        int n_campos_busca = 0;
        scanf(" %d", &n_campos_busca);
        
        // Ler as informações de busca
        string_t *nome_campos_busca = (string_t *)malloc(n_campos_busca * sizeof(string_t));
        string_t *valor_campos_busca = (string_t *)malloc(n_campos_busca * sizeof(string_t));

        for (int i = 0; i < n_campos_busca; i++) {
            nome_campos_busca[i] = scan_quote_string();
            valor_campos_busca[i] = scan_quote_string();
        }

        // Ler quantos campos de alteração serão usados
        int n_campos_alteracao = 0;
        scanf(" %d", &n_campos_alteracao);
        
        // Ler informações de alteração
        string_t *nome_campos_alteracao = (string_t *)malloc(n_campos_alteracao * sizeof(string_t));
        string_t *valor_campos_alteracao = (string_t *)malloc(n_campos_alteracao * sizeof(string_t));

        for (int i = 0; i < n_campos_alteracao; i++) {
            nome_campos_alteracao[i] = scan_quote_string();
            valor_campos_alteracao[i] = scan_quote_string();
        }

        if (compare_strings_case_sensitive(nome_campos_busca[0], "id") == 0) { // Alterar buscando pelo índice primário
            int id_a_remover = atoi(valor_campos_busca[0]);

            long long int posicao_de_alteracao = busca_no_indice(indice, tipo_do_arquivo, id_a_remover, NULL);

            if (posicao_de_alteracao == -1) { // id não existe
                destroy_string_array(nome_campos_busca, n_campos_busca);
                destroy_string_array(valor_campos_busca, n_campos_busca);
                destroy_string_array(nome_campos_alteracao, n_campos_alteracao);
                destroy_string_array(valor_campos_alteracao, n_campos_alteracao);
                continue; 
            }

            if (n_campos_busca == 1) { // Se o campo for apenas id, alterar na posição lida do índice
                alterar_do_arquivo_por_posicao(tipo_do_arquivo, arq_entrada, cabecalho, indice, posicao_de_alteracao, nome_campos_alteracao, valor_campos_alteracao, n_campos_alteracao);                
            } else { // Se houverem mais campos de especificações, testá-los no registro buscado antes de alterar
                registro_t *reg = get_registro_em_posicao(tipo_do_arquivo, posicao_de_alteracao, arq_entrada);

                if (registro_encontrado(reg, nome_campos_busca, valor_campos_busca, n_campos_busca)) {
                    alterar_do_arquivo_por_posicao(tipo_do_arquivo, arq_entrada, cabecalho, indice, posicao_de_alteracao, nome_campos_alteracao, valor_campos_alteracao, n_campos_alteracao);                
                }

                destruir_registro(reg, 1);
            }
        } else { // Leitura sequencial do arquivo alterando caso registro bata com as especificações
            alterar_sequencialmente(tipo_do_arquivo, arq_entrada, cabecalho, indice, nome_campos_busca, valor_campos_busca, n_campos_busca, nome_campos_alteracao, valor_campos_alteracao, n_campos_alteracao);
        }

        destroy_string_array(nome_campos_busca, n_campos_busca);
        destroy_string_array(valor_campos_busca, n_campos_busca);
        destroy_string_array(nome_campos_alteracao, n_campos_alteracao);
        destroy_string_array(valor_campos_alteracao, n_campos_alteracao);
    }

    // Arquivo consistente após todas as alterações realizadas:
    set_status(cabecalho, '1'); 
    escrever_cabecalho_em_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    destruir_cabecalho(cabecalho);

    // Escrever índice em RAM em arquivo
    escrever_indice(indice, tipo_do_arquivo, arquivo_de_indice);
    destruir_indice(indice);

    fclose(arq_entrada);

    // Imprimir binário na tela de Arquivo de Dados e Arquivo de Índice, respectivamente
    binarioNaTela(binario_entrada);
    binarioNaTela(arquivo_de_indice);
}

// Cria um arquivo de índice primário árvore B a partir de um arquivo de dados existente de tipo indicado
// A chave primária é o campo id do registros de dados
// Para o tipo1, guarda-se para cada chave primária o RRN do registro no arquivo de dados
// Para o tipo2, guarda-se para cada chave primária o Byteoffset do registro no arquivo de dados
void funcionalidade9(int tipo_do_arquivo, string_t arquivo_de_dados, string_t arquivo_de_indice) {
    FILE *arq_entrada = NULL;

    arq_entrada = fopen(arquivo_de_dados, "rb");

    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");

        return;
    }
    
    // Ler do cabecalho status, proxRRN e proxByteOffset
    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    char status = get_status(cabecalho);
    int proxRRN = get_proxRRN(cabecalho);
    long long int proxByteOffset = get_proxByteOffset(cabecalho);

    destruir_cabecalho(cabecalho);
    
    // Testar consistência do arquivo
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(arq_entrada);

        return;
    }

    //---------------------------------------------------------------------------------

    // Criando arquivo de árvore para leitura e escrita:
    FILE *arq_arvore = NULL;

    arq_arvore = fopen(arquivo_de_indice, "w+b");

    if (!arq_arvore) {
        printf("Falha no processamento do arquivo.\n");

        return;
    }

    // Criar cabeçalho de árvore inicializado com valores padrão:
    cabecalho_arvore_t cab_arvore = criar_cabecalho_arvore(); // status '0' (inconsistente)
    // Escrever cabeçalho inicialmente inconsistente
    escrever_cabecalho_arvore(&cab_arvore, tipo_do_arquivo, arq_arvore);

    // Ler sequencialmente o arquivo de dados salvando as informações no índice em RAM:

    if (tipo_do_arquivo == 1) {
        // Leitura sequencial dos registros adicionando no índice as informações dos não removidos:
        for (int rrn = 0; rrn < proxRRN; rrn++) {
            registro_t *reg = criar_registro();

            ler_registro(reg, tipo_do_arquivo, arq_entrada);
            
            char removido = get_removido(reg);

            if (removido == '0') {
                chave_t nova_chave = {.id = get_id(reg), .RRN_dados = rrn};
                inserir_chave_em_arvore(&nova_chave, tipo_do_arquivo, &cab_arvore, arq_arvore);
            }

            destruir_registro(reg, 1);
        }
    } else if (tipo_do_arquivo == 2) {
        // Leitura sequencial dos registros adicionando no índice as informações dos não removidos:
        long long int byteOffset_atual = TAM_CAB_2;
        while (byteOffset_atual < proxByteOffset) {
            registro_t *reg = criar_registro();

            int tam_reg = ler_registro(reg, tipo_do_arquivo, arq_entrada);

            char removido = get_removido(reg);

            if (removido == '0') {
                chave_t nova_chave = {.id = get_id(reg), .byteoffset_dados = byteOffset_atual};
                inserir_chave_em_arvore(&nova_chave, tipo_do_arquivo, &cab_arvore, arq_arvore);
            }

            byteOffset_atual += tam_reg;
            destruir_registro(reg, 1);
        }
    }    

    // Escrever o cabeçalho da árvore atualizado
    cab_arvore.status = '1';
    escrever_cabecalho_arvore(&cab_arvore, tipo_do_arquivo, arq_arvore);
    fclose(arq_arvore);
    fclose(arq_entrada);

    // Imprimir "identificação" do arquivo de índice árvore-B
    binarioNaTela(arquivo_de_indice);
}

// Insere no arquivo de dados e de índice árvore-B correspondente 'n' novos registro lidos entrada padrão
void funcionalidade11(int tipo_do_arquivo, string_t binario_entrada, string_t arquivo_de_indice, int n_insercoes) {
    // Abrir arquivo para leitura e escrita de binário
    FILE *arq_entrada = fopen(binario_entrada, "r+b");
    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler cabecalho e testar consistência
    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    char status = get_status(cabecalho);
    
    // Testar consistência do arquivo
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");
        destruir_cabecalho(cabecalho);
        fclose(arq_entrada);

        return;
    }

    // Arquivo potencialmente inconsistente neste ponto
    set_status(cabecalho, '0');
    escrever_cabecalho_em_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);

    // Abrir arquivo de índice árvore-B para leitura e escrita
    FILE *arq_arvore = NULL;

    arq_arvore = fopen(arquivo_de_indice, "r+b");

    if (!arq_arvore) {
        printf("Falha no processamento do arquivo.\n");
        destruir_cabecalho(cabecalho);
        fclose(arq_entrada);

        return;
    } 

    //---------------------------------------------------------------------------------

    // Lê cabeçalho da arvore e testa a consistência
    cabecalho_arvore_t cab_arvore = ler_cabecalho_arvore(arq_arvore, tipo_do_arquivo);

    if (cab_arvore.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        destruir_cabecalho(cabecalho);
        fclose(arq_entrada);
        fclose(arq_arvore);

        return;
    }

    // Escrever cabeçalho da árvore inicialmente inconsistente
    cab_arvore.status = '0';
    escrever_cabecalho_arvore(&cab_arvore, tipo_do_arquivo, arq_arvore);

    // Laço que realiza cada uma das inserções especificadas na entrada padrão
    for (int insercao = 0; insercao < n_insercoes; insercao++) {
        // Ler campos de novo registro da entrada padrão
        string_t *campos = (string_t *)malloc(7 * sizeof(string_t));

        for (int i = 0; i < 7; i++) {
            campos[i] = scan_quote_string();
        }

        // Cria preenche e insere chave no arquivo de dados e índice árvore-B
        registro_t *registro_a_adicionar = criar_registro();
        preencher_registro(registro_a_adicionar, campos);
        inserir_registro_arvore(tipo_do_arquivo, arq_entrada, cabecalho, arq_arvore, &cab_arvore, registro_a_adicionar);

        destroy_string_array(campos, 7);
        destruir_registro(registro_a_adicionar, 0);
    }

    // Arquivo consistente após todas as alterações realizadas:
    set_status(cabecalho, '1'); 
    escrever_cabecalho_em_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    destruir_cabecalho(cabecalho);

    // Escrever o cabeçalho da árvore atualizado
    cab_arvore.status = '1';
    escrever_cabecalho_arvore(&cab_arvore, tipo_do_arquivo, arq_arvore);
    fclose(arq_arvore);
    fclose(arq_entrada);

    // Imprimir binário na tela de Arquivo de Dados e Arquivo de Índice, respectivamente
    binarioNaTela(binario_entrada);
    binarioNaTela(arquivo_de_indice);
}

// Busca um id na árvore-B e em seguida imprime o registro encontrado
void funcionalidade10(int tipo_do_arquivo, string_t binario_entrada, string_t arquivo_de_indice, int id) {
    // Abrir arquivo para leitura de binário
    FILE *arq_entrada = fopen(binario_entrada, "rb");
    if (!arq_entrada) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Ler cabecalho e testar consistência
    cabecalho_t *cabecalho = criar_cabecalho();

    ler_cabecalho_de_arquivo(cabecalho, tipo_do_arquivo, arq_entrada);
    char status = get_status(cabecalho);
    destruir_cabecalho(cabecalho);
    
    // Testar consistência do arquivo
    if (status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(arq_entrada);

        return;
    }

    // Abrir arquivo de índice árvore-B para leitura
    FILE *arq_arvore = NULL;

    arq_arvore = fopen(arquivo_de_indice, "rb");

    if (!arq_arvore) {
        printf("Falha no processamento do arquivo.\n");
        fclose(arq_entrada);

        return;
    } 

    //---------------------------------------------------------------------------------

    // A função buscar_por_id_na_arvore retorna a posicao do registro na árvore-B
    // Se o arquivo for do tipo 1, retorna o rrn
    // Se for do tipo 2, retorna o byteoffset
    long long int pos_reg_buscado = buscar_por_id_na_arvore(id, tipo_do_arquivo, arq_arvore);

    if (pos_reg_buscado == NIL) {
        printf("Registro inexistente.\n");
    } else {
        registro_t *reg = criar_registro();
        long long int byte_offset = pos_reg_buscado;
        if (tipo_do_arquivo == 1) {
            byte_offset = TAM_CAB_1 + pos_reg_buscado * TAM_REG1;
        }
        fseek(arq_entrada, byte_offset, SEEK_SET);
        ler_registro(reg, tipo_do_arquivo, arq_entrada);
        imprimir_registro(reg);
        destruir_registro(reg, 1);
    }

    fclose(arq_arvore);
    fclose(arq_entrada);
}