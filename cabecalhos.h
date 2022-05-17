#define TAM_CAB_1 182
#define TAM_CAB_2 190

typedef struct cabecalho_ cabecalho_t;

// Aloca e retorna cabeçalho inicializado com valores padrão
cabecalho_t *criar_cabecalho();

// Escreve um cabeçalho do tipo indicado no início de um arquivo já aberto com permissão para escrita
void escrever_cabecalho_em_arquivo(cabecalho_t *cabecalho, int tipo_do_arquivo, FILE *fp);

// Lê cabeçalho tipo indicado de arquivo já aberto com permissão para leitura
void ler_cabecalho_de_arquivo(cabecalho_t *cabecalho, int tipo_do_arquivo, FILE *fp);

// Desaloca memória relacionada ao cabeçalho
void destruir_cabecalho1(cabecalho_t *cabecalho);

// Getters e Setters:
void set_status(cabecalho_t *cabecalho, char status);
char get_status(cabecalho_t *cabecalho);
void set_topo(cabecalho_t *cabecalho, long long int topo);
long long int get_topo(cabecalho_t *cabecalho);
void set_proxRRN(cabecalho_t *cabecalho, int proxRRN);
int get_proxRRN(cabecalho_t *cabecalho);
void set_nroRegRem(cabecalho_t *cabecalho, int nroRegRem);
int get_nroRegRem(cabecalho_t *cabecalho)
void set_proxByteOffset(cabecalho_t *cabecalho, long long int proxByteOffset)
long long int get_proxByteOffset(cabecalho_t *cabecalho)