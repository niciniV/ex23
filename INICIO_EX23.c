#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Nomes: Diogo Gonçalves Bonofre dos Santos (202413177) e Vinícius Amorim da Silveira (202413352)
 * Responsabilidades: Diogo foi responsável por desenvolver as funções de soma dos salários e desalocação
 * de memória. Vinícius foi responsável pela implementação das funções de busca por valor e nome. Os 2
 * contribuíram um com o outro em dicas e dúvidas pontuais sobre as implementações e no processo de debug.
 * 
 * Considere uma árvore (nao vetor lista encadeada grafo)
 * 
 * binária de busca.
 * 
 * A função insere já foi escrita pelo professor e ela precisa ler
 * o arquivo que já tem os dados a serem cadastros. Deixe o arquivo
 * na mesma pasta do código.
 * 
 * Você pode construir um arquivo de dados em TXT:
 * valor
 * salario
 * nome
 * valor
 * salario
 * nome
 * ....
 * 
 * Serão fornecidos alguns arquivos prontos 
 * 
 * Implemente as seguintes funções:
 * - função busca: ela retorna um ponteiro para o nó onde está a chave
 *   buscada ou NULL se não existe. A chave é o valor
 * - função buscaNome: ela retorna um ponteiro para o nó onde tem este
 *   nome ou NULL se não existe. Apesar do nome não ser a chave, considere
 *   que não há nomes repetidos.
 * - funcão contabilidade: deve retornar a soma de todos os salários
 * - desaloca: ao final, realizar a desalocação de todos os nós da árvore.
 * 
 * */

/* Estrutura "elgio complica IA". Varios arquivos, dificultando ou tornando
 * mais trabalhosa a solucao automatica por IA. Usem a IA para aprender (super
 * apoio) mas não para dar a resposta pronta o entregar ela sem nem olhar 
 * ou tentar entender o que fez
 * */

/* arquivo com definicoes da strutura e outros defines
 * tenha ele na mesma pasta deste codigo
 * */
#include "definicoes.h"

/* arquivo com as funcoes já programadas pelo professor
 * tenha ele na mesma pasta deste codigo
 * */
#include "funcoesProf.c"

/* Eu ja tenho as respostas. As tres funcoes prontas no meu 
 * arquivo resp.c. Nao liberei ele, está só comigo.
 * 

 * Se definir a macro GABARITO tenta incluir as respostas
 * Uso para meus testes aqui.
 * */
#ifdef GABARITO
#include "resp.c"
#endif


ARVORE *busca_valor(ARVORE *r, int valor) {
    if(r == NULL) return NULL;

    if(r->valor == valor) return r;

    if(r->valor > valor) return busca_valor(r->e, valor);
    return busca_valor(r->d, valor);
}

ARVORE *busca_nome(ARVORE *r, const char *nome) {
    if(r == NULL) return NULL;
    if(strcmp(r->nome, nome) == 0) return r;

    ARVORE *temp;

    temp = busca_nome(r->e, nome);
    if(temp) return temp;
    temp = busca_nome(r->d, nome);
    if(temp) return temp;
    return NULL;
}

double contabiliza(ARVORE *r) {
  if (r == NULL) return 0;

  double soma_e = contabiliza(r->e);
  double soma_d = contabiliza(r->d);

  return r->salario + soma_e + soma_d;
}

void desaloca(ARVORE *r) {
  if (r == NULL) return;
  
  desaloca(r->e);
  desaloca(r->d);
  
  // DEBUG 
  // printf("Liberando nó com salário: %.2f (Endereço: %p)\n", r->salario, (void*)r);
  free(r);
}

int main(int argc, char *argv[])
{
    ARVORE *r;                  // A raiz da árvore
    ARVORE *no;                 // o no atual
    ARVORE *resp;
    double totsal;
    
    int total = 0;
    FILE *arq;
    int valor;
    char nome[MAXSTR+1];

    r = NULL;
    resp = NULL;
    no = NULL;
    
    if (argc < 2) {
        fprintf(stderr, "ERRO. Você precisa fornecer o nome do arquivo com os dados\n");
        fprintf(stderr, "Sintaxe: %s arquivo.txt\n", argv[0]);
        return (1);
    }
    arq = fopen(argv[1], "r");
    if (arq == NULL) {
        fprintf(stderr, "ERRO. Não pude abrir o arquivo \"%s\" para leitura\n", argv[1]);
        return (2);
    }
    while (!feof(arq)) {
        /* A técnica aqui é primeiro ler os dados e colocar ele em um nó folha sozinho,
         * sem pai, para depois encaixar ele na árvore já existente.
         * A função ledados apenas lê um dado do arquivo e coloca dentro do nó
         */
        no = ledados(arq);
        if (no == NULL) {
            /* não pôde criar o no. A função ledados já printou o motivo aqui apenas 
             * encerra o laço, pois não ter um nó é porque acabou o arquivo
             */
            break;
        }

        /* Dados do no lido. Agora resta inserir ele na arvore */
        if (r == NULL) {
            /* arvore vazia. Entào este no será o raiz e pronto */
            r = no;
            total = 1;
            continue;           // volta no laco que le o arquivo
        }
        if (insere(no, r) == NULL) {
            // Não pode ser inserido. Já existe?
            free(no);
        } else {
			total++;
		}
    }
    fclose(arq);
    
    /* Aqui a árvore foi criada com os dados do arquivo. É apenas uma árvore binária 
     * de busca. Pode até ser uma zigzag. Depende de como os dados estavam no arquivo.
     * As funções de cadastro aqui apenas garantem ser de busca.
     * 
     * Elas não tentam manter a árvore como (quase)completa
     * */

    printf("\n==================================\n");
    printf("Lido %d nos do arquivo \"%s\"", total, argv[1]);
    printf("\n==================================\n\n");
    

/* CUIDADO: para arvores grandes os prints de depuracao a seguir irao gerar
 * muita impressao. Use apenas para testar em arvores pequenas
 * comente em caso de arvores com muitos dados
 * */
    printf("Depuração: percurso pre-ordem\n");
    pre(r);
    printf("\n\n");

    printf("Depuração: percurso em em-ordem\n");
    em(r);
    printf("\n\n");

    printf("Depuração: percurso em pós-ordem\n");
    pos(r);
    printf("\n\n");

    /* Aqui começa vocês:
     * 
     * Implemente as seguintes funções:
     * - função busca: ela retorna um ponteiro para o nó onde está a chave
     *   buscada ou NULL se não existe. A chave é o valor
     * - função buscaNome: ela retorna um ponteiro para o nó onde tem este
     *   nome ou NULL se não existe. Apesar do nome não ser a chave, considere
     *   que não há nomes repetidos.
     * - funcão contabilidade: deve retornar a soma de todos os salários
     * - desaloca: ao final, realizar a desalocação de todos os nós da árvore.
     * 
     * Já dei sugestões de leitura e da invocação das funções.
     * Mas a partir daqui vocês adaptam do seu jeito.
     */
    
    
    printf("Digite um valor a ser buscado:\n");
    scanf("%d", &valor);
    
    /* Comentado abaixo porque não existe a função busca_valor
     * Você deve inmplementar ela
     * */
    resp = busca_valor(r, valor);
	
	if (resp == NULL){
		printf("%d não encontrado na árvore\n", valor);
	} else {
		printf("Achei %d\n", valor);
		imprimeNO(resp);
	}
	
    printf("Digite um nome a ser encontrado:\n");
    lestringARQ(nome, MAXSTR, stdin);
    
    /* Comentado abaixo porque não existe a função busca_nome
     * Você deve implementar ela
     * */
    resp = busca_nome(r, nome);

    if (resp == NULL) {
      printf("%s não encontrado na árvore\n", nome);
    } else {
      printf("Achei %d\n", resp->valor);
      imprimeNO(resp);
    }
    
    /* A funcao contabiliza você implementa. Ela deve retornar a soma 
     * de todos os salários
     * */	
    totsal = contabiliza(r);
    printf("Total de salarios = R$ %7.2lf\n", totsal);
	
    // DESALOCA
    desaloca(r);

    return 0;
}

