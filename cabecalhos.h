#define TAM_CAB_1 182
#define TAM_CAB_2 190

typedef struct cabecalho1_ cabecalho1_t;
typedef struct cabecalho2_ cabecalho2_t;

/* Funções para Cabeçalho Tipo 1: */

// Aloca e retorna cabeçalho tipo 1 inicializado com valores padrão
cabecalho1_t *criar_cabecalho1();

// Getters e Setters:
void set_status1(cabecalho1_t *c1, char status);
char get_status1(cabecalho1_t *c1);
void set_topo1(cabecalho1_t *c1, int topo);
int get_topo1(cabecalho1_t *c1);
void set_proxRRN1(cabecalho1_t *c1, int proxRRN);
int get_proxRRN1(cabecalho1_t *c1);
void set_nroRegRem1(cabecalho1_t *c1, int nroRegRem);
int get_nroRegRem1(cabecalho1_t *c1);

// Escreve um cabeçalho tipo 1 no início de um arquivo já aberto com permissão para escrita
void escrever_cabecalho1_em_arquivo(cabecalho1_t *c1, FILE *fp);

// Lê cabeçalho tipo 1 de arquivo já aberto com permissão para leitura
void ler_cabecalho1_de_arquivo(cabecalho1_t *c1, FILE *fp);

void destruir_cabecalho1(cabecalho1_t *c1);

/* Funções para Cabeçalho Tipo 2: */

// Aloca e retorna cabeçalho tipo 2 inicializado com valores padrão
cabecalho2_t *criar_cabecalho2();

// Getters e Setters:
void set_status2(cabecalho2_t *c2, char status);
char get_status2(cabecalho2_t *c2);
void set_topo2(cabecalho2_t *c2, long long int topo);
long long int get_topo2(cabecalho2_t *c2);
void set_proxByteOffset2(cabecalho2_t *c2, long long int proxByteOffset);
long long int get_proxByteOffset2(cabecalho2_t *c2);
void set_nroRegRem2(cabecalho2_t *c2, int nroRegRem);
int get_nroRegRem2(cabecalho2_t *c2);

// Escreve um cabeçalho tipo 2 no início de um arquivo já aberto com permissão para escrita
void escrever_cabecalho2_em_arquivo(cabecalho2_t *c2, FILE *fp);

// Lê cabeçalho tipo 2 de arquivo já aberto com permissão para leitura
void ler_cabecalho2_de_arquivo(cabecalho2_t *c2, FILE *fp);

void destruir_cabecalho2(cabecalho2_t *c2);