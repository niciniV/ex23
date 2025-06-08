#define MAXSTR   100
#define MAXLINHA 10000

typedef struct ARV {
    int valor;
    double salario;
    char nome[MAXSTR+1];
    struct ARV *e;
    struct ARV *d;
} ARVORE;

// PROTOTYPE DAS FUNÇÕES
ARVORE *ledados(FILE * arq);
ARVORE *insere(ARVORE *, ARVORE *);
int lestringARQ(char *, int, FILE *);
int pre(ARVORE *);
int em(ARVORE *);
int pos(ARVORE *);
int imprimeNO(ARVORE *);

