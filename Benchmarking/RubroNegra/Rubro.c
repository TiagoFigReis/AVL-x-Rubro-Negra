//
// Created by tiago on 07/06/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "Rubro.h"

typedef struct no {
    int chave;
    struct no *Fdir;
    struct no *Fesq;
    struct no *pai;
    char cor;
} no;

typedef struct arvore {
    struct no *sentinela;
    struct no *nulo;
    int numElementos;
} arv;

arv *criaArvoreRubro() {
    arv *rubro = (arv*)malloc(sizeof(arv));
    if (!rubro)
        return NULL;

    rubro->sentinela = (no*)malloc(sizeof(no));
    rubro->nulo = (no*)malloc(sizeof(no));

    if (!rubro->sentinela || !rubro->nulo)
        return NULL;

    rubro->sentinela->Fdir = rubro->nulo;
    rubro->sentinela->Fesq = rubro->nulo;
    rubro->sentinela->pai = rubro->nulo;
    rubro->sentinela->chave = -1000;
    rubro->sentinela->cor = 'P';

    rubro->nulo->Fdir = NULL;
    rubro->nulo->Fesq = NULL;
    rubro->nulo->pai = NULL;
    rubro->nulo->chave = -1000;
    rubro->nulo->cor = 'P';

    rubro->numElementos = 0;
    return rubro;
}

int inserir(arv *arv, int valor, long int *count){
    no *n, *aux, *ant;
    n = (no*)malloc(sizeof(no));
    if(!n)
        return 0;
    n->cor = 'V';
    n->Fdir = arv->nulo;
    n->Fesq = arv->nulo;
    n->chave = valor;

    if (arv->sentinela->Fdir == arv->nulo){
        arv->sentinela->Fdir = n;
        n->pai = arv->sentinela;
        n->cor = 'P';
        arv->numElementos++;
        return 1;
    }
    aux = arv->sentinela->Fdir;
    while(aux != arv->nulo){
        ant = aux;
        if (aux->chave <= valor)
            aux = aux->Fdir;
        else
            aux = aux->Fesq;
    }
    if (ant->chave <= valor){
        ant->Fdir = n;
    }else{
        ant->Fesq = n;
    }
    n->pai = ant;
    balanceamentoInsercao(arv, n, count);
    arv->numElementos++;
    return 1;
}

int remover(arv *arv, int valor, long int *count) {
    no *aux = arv->sentinela->Fdir, *sucessor, *x;
    char corOriginal;

    while (aux != arv->nulo && aux->chave != valor) {
        if (valor < aux->chave) {
            aux = aux->Fesq;
        } else {
            aux = aux->Fdir;
        }
    }

    if (aux == arv->nulo)
        return 0;

    arv->numElementos--;

    corOriginal = aux->cor;

    if (aux->Fesq == arv->nulo) {
        x = aux->Fdir;
        if (aux == aux->pai->Fesq) {
            aux->pai->Fesq = aux->Fdir;
        } else {
            aux->pai->Fdir = aux->Fdir;
        }
        aux->Fdir->pai = aux->pai;
    } else if (aux->Fdir == arv->nulo) {
        x = aux->Fesq;
        if (aux == aux->pai->Fesq) {
            aux->pai->Fesq = aux->Fesq;
        } else {
            aux->pai->Fdir = aux->Fesq;
        }
        aux->Fesq->pai = aux->pai;
    } else {
        sucessor = aux->Fdir;
        while (sucessor->Fesq != arv->nulo) {
            sucessor = sucessor->Fesq;
        }
        corOriginal = sucessor->cor;
        x = sucessor->Fdir;

        if (sucessor->pai == aux) {
            x->pai = sucessor;
        } else {
            if (sucessor == sucessor->pai->Fesq) {
                sucessor->pai->Fesq = sucessor->Fdir;
            } else {
                sucessor->pai->Fdir = sucessor->Fdir;
            }
            sucessor->Fdir->pai = sucessor->pai;
            sucessor->Fdir = aux->Fdir;
            sucessor->Fdir->pai = sucessor;
        }
        if (aux == aux->pai->Fesq) {
            aux->pai->Fesq = sucessor;
        } else {
            aux->pai->Fdir = sucessor;
        }
        sucessor->pai = aux->pai;
        sucessor->Fesq = aux->Fesq;
        sucessor->Fesq->pai = sucessor;
        sucessor->cor = aux->cor;
    }

    if (corOriginal == 'P') {
        balanceamentoRemocao(arv, x, count);
    }

    free(aux);
    return 1;
}

int BuscaRubro(arv *arv, int valor){
    no *aux;
    aux = arv->sentinela->Fdir;
    if(!aux)
        return -1;
    while(aux != arv->nulo && aux->chave != valor){
        if(aux->chave<=valor)
            aux = aux->Fdir;
        else
            aux = aux->Fesq;
    }
    if(aux == arv->nulo)
        return 0;
    else
        return 1;
}

void balanceamentoInsercao(arv *arv, no *n, long int *count) {
    no *tio;
    while (n->pai->cor == 'V') {
        if (n->pai == n->pai->pai->Fesq) {
            tio = n->pai->pai->Fdir;
            if (tio->cor == 'V') {
                n->pai->cor = 'P';
                tio->cor = 'P';
                n->pai->pai->cor = 'V';
                n = n->pai->pai;
            } else {
                (*count)++;
                if (n == n->pai->Fdir) {
                    n = n->pai;
                    rotacaoesq(arv, n);
                }
                n->pai->cor = 'P';
                n->pai->pai->cor = 'V';
                rotacaodir(arv, n->pai->pai);
            }
        } else {
            tio = n->pai->pai->Fesq;
            if (tio->cor == 'V') {
                n->pai->cor = 'P';
                tio->cor = 'P';
                n->pai->pai->cor = 'V';
                n = n->pai->pai;
            } else {
                (*count)++;
                if (n == n->pai->Fesq) {
                    n = n->pai;
                    rotacaodir(arv, n);
                }
                n->pai->cor = 'P';
                n->pai->pai->cor = 'V';
                rotacaoesq(arv, n->pai->pai);
            }
        }
    }
    arv->sentinela->Fdir->cor = 'P';
}

void balanceamentoRemocao(arv *arv, no *n, long int *count) {
    no *irmao;
    while (n != arv->sentinela->Fdir && n->cor == 'P') {
        if (n == n->pai->Fesq) {
            irmao = n->pai->Fdir;
            if (irmao->cor == 'V') {
                (*count)++;
                irmao->cor = 'P';
                n->pai->cor = 'V';
                rotacaoesq(arv, n->pai);
                irmao = n->pai->Fdir;
            }
            if (irmao->Fesq->cor == 'P' && irmao->Fdir->cor == 'P') {
                irmao->cor = 'V';
                n = n->pai;
            } else {
                (*count)++;
                if (irmao->Fdir->cor == 'P') {
                    irmao->Fesq->cor = 'P';
                    irmao->cor = 'V';
                    rotacaodir(arv, irmao);
                    irmao = n->pai->Fdir;
                }
                irmao->cor = n->pai->cor;
                n->pai->cor = 'P';
                irmao->Fdir->cor = 'P';
                rotacaoesq(arv, n->pai);
                n = arv->sentinela->Fdir;
            }
        } else {
            irmao = n->pai->Fesq;
            if (irmao->cor == 'V') {
                (*count)++;
                irmao->cor = 'P';
                n->pai->cor = 'V';
                rotacaodir(arv, n->pai);
                irmao = n->pai->Fesq;
            }
            if (irmao->Fesq->cor == 'P' && irmao->Fdir->cor == 'P') {
                irmao->cor = 'V';
                n = n->pai;
            } else {
                (*count)++;
                if (irmao->Fesq->cor == 'P') {
                    irmao->Fdir->cor = 'P';
                    irmao->cor = 'V';
                    rotacaoesq(arv, irmao);
                    irmao = n->pai->Fesq;
                }
                irmao->cor = n->pai->cor;
                n->pai->cor = 'P';
                irmao->Fesq->cor = 'P';
                rotacaodir(arv, n->pai);
                n = arv->sentinela->Fdir;
            }
        }
    }
    n->cor = 'P';
}

void rotacaoesq(arv *arv, no *x) {
    no *y = x->Fdir;
    x->Fdir = y->Fesq;
    if (y->Fesq != arv->nulo) {
        y->Fesq->pai = x;
    }
    y->pai = x->pai;
    if (x == x->pai->Fesq) {
        x->pai->Fesq = y;
    } else {
        x->pai->Fdir = y;
    }
    y->Fesq = x;
    x->pai = y;
}

void rotacaodir(arv *arv, no *y) {
    no *x = y->Fesq;
    y->Fesq = x->Fdir;
    if (x->Fdir != arv->nulo) {
        x->Fdir->pai = y;
    }
    x->pai = y->pai;
    if (y == y->pai->Fdir) {
        y->pai->Fdir = x;
    } else {
        y->pai->Fesq = x;
    }
    x->Fdir = y;
    y->pai = x;
}

int getNumElementosRubro(arv *arv){
    return arv->numElementos;
}

no *getRaizRubro(arv *arv){
    return arv->sentinela->Fdir;
}

void imprimePreOrdemRubro(arv *arv, no *raiz){
    if(raiz != arv->nulo){
        printf("%d - %d - %c\n",raiz->chave, raiz->pai->chave, raiz->cor);
        imprimePreOrdemRubro(arv, raiz->Fesq);
        imprimePreOrdemRubro(arv, raiz->Fdir);
    }
}

void imprimeOrdemRubro(arv *arv, no *raiz){
    if (raiz != arv->nulo){
        imprimeOrdemRubro(arv, raiz->Fesq);
        printf("%d - %d - %c\n",raiz->chave, raiz->pai->chave, raiz->cor);
        imprimeOrdemRubro(arv, raiz->Fdir);
    }
}