#define TAM_CAB_ARV 13
#define TAM_REG1_ARV 45
#define TAM_REG2_ARV 57

#define NIL -1

enum valor_de_retorno {
    SEM_PROMO,
    PROMO,
    ERRO
};

enum tipo_no_ {
    RAIZ = '0',
    INTERMEDIARIO = '1',
    FOLHA = '2'
};

typedef struct cabecalho_arvore_ cabecalho_arvore_t;
typedef struct chave_ chave_t;
typedef struct no_arvore_ no_arvore_t;

struct cabecalho_arvore_ {
    char status;
    int noRaiz;
    int proxRRN;
    int nroNos;
};

struct chave_ {
    int id;
    int RRN_dados;
    long long int byteoffset_dados;
};

struct no_arvore_ {
    int RRN_arvore;
    char tipoNo;
    int nroChaves;
    chave_t chaves[3+1]; // 1 a mais para split
    int ponteiros[4+1]; // 1 a mais para split
};

// Retorna cabecalho de arvore inicializado com valores padrão
cabecalho_arvore_t criar_cabecalho_arvore();

// Retorna nó de arvore inicializado com valores padrão
no_arvore_t criar_no_arvore();

// Escreve cabelho em arquivo de árvore já aberto com permissão para escrita
void escrever_cabecalho_arvore(cabecalho_arvore_t *cab_arvore, int tipo_do_arquivo, FILE *arq_arvore);

// Lê cabecalho de arquivo de árvore já aberto com permissão para leitura
cabecalho_arvore_t ler_cabecalho_arvore(FILE *arq_arvore, int tipo_do_arquivo);

// Escreve nó em arquivo de árvore de tipo indicado já aberto com permissão para leitura
void escrever_no_arvore(no_arvore_t *no, int tipo_do_arquivo, FILE *arq_arvore);

// Lê nó de arquivo de árvore de tipo indicado já aberto com permissão para escrita
no_arvore_t ler_no_arvore(int tipo_do_arquivo, FILE *arq_arvore);

// Busca por chave com id indicado na árvore toda.
// Retorna -1 se não encontrou ou a referência adequada para o arquivo de dados (RRN ou Byteoffset).
// Retorna por referência o indice:
//      Caso o id tenha sido encontrado, ele representa a posição no vetor de chaves do nó em que ele foi encontrado.
//      Caso o id não tenha sido encontrado, ele represente a posição no vetor de ponteiros do ponteiro que a busca deve seguir.
// Também retorna por referência o ultimo nó analisado, atualizando o campo que indica RRN no arquivo da árvore.
long long int buscar_por_id_na_arvore(int id, int tipo_do_arquivo, FILE *arq_arvore, int *indice, no_arvore_t *ultimo_no);

void inserir_chave_em_arvore(chave_t *chave_a_inserir, int tipo_do_arquivo, cabecalho_arvore_t *cab_arvore, FILE *arq_arvore);

void split(chave_t *nova_chave, int *filho_direito, no_arvore_t *no, chave_t *chave_promo, int *rrn_filho_dir_promo, no_arvore_t *novo_no, int tipo_do_arquivo, cabecalho_arvore_t *cab_arvore);