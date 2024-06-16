//
// Created by tiago on 07/06/2024.
//

#ifndef EX_9_RUBRO_NEGRA_RUBRO_H
#define EX_9_RUBRO_NEGRA_RUBRO_H
typedef struct no no;

typedef struct arvore arv;

arv *criaArvoreRubro();

int inserir(arv *arv, int valor, long int *count);

int remover(arv *arv, int valor, long int *count);

int BuscaRubro(arv *arv, int valor);

void balanceamentoInsercao(arv *arv, no *n, long int *count);

void balanceamentoRemocao(arv *arv, no *n, long int *count);

void rotacaodir(arv *arv, no *n);

void rotacaoesq(arv *arv, no *n);

int getNumElementosRubro(arv *arv);

no *getRaizRubro(arv *arv);

void imprimePreOrdemRubro(arv *arv, no *raiz);

void imprimeOrdemRubro(arv *arv, no *raiz);
#endif //EX_9_RUBRO_NEGRA_RUBRO_H
