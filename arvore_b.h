#define TAM_CAB_ARV 13
#define TAM_REG1_ARV 45
#define TAM_REG2_ARV 57

enum tipo_no_ {
    RAIZ = 0,
    INTERMEDIARIO = 1,
    FOLHA = 2
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
    int tipoNo;
    int nroChaves;
    chave_t chaves[3];
    int ponteiros[4];
};