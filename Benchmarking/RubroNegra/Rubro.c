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
    struct no *nulo; // Sentinela que os nós irão apontar ao invés de apontar pra nulo
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

    // Inicializando a sentinela
    rubro->sentinela->Fdir = rubro->nulo;
    rubro->sentinela->Fesq = rubro->nulo;
    rubro->sentinela->pai = rubro->nulo;
    rubro->sentinela->chave = -1000;
    rubro->sentinela->cor = 'P';

    // Inicializando a sentinela nula
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
    // Nó inserido sempre vermelho
    n->cor = 'V';
    // Ao invés de apontar pra NULL, apontar pra sentinela nula
    n->Fdir = arv->nulo;
    n->Fesq = arv->nulo;
    n->chave = valor;

    if (arv->sentinela->Fdir == arv->nulo){
        // Árvore sem nenhum nó, nó inserido é a raiz da árvore
        arv->sentinela->Fdir = n;
        n->pai = arv->sentinela;
        n->cor = 'P';
        arv->numElementos++;
        return 1;
    }
    aux = arv->sentinela->Fdir;
    // Percorrer a árvore para encontrar o lugar onde deve ser inserido o novo nó
    while(aux != arv->nulo){
        // Guardando o pai do aux
        ant = aux;
        if (aux->chave <= valor)
            aux = aux->Fdir;
        else
            aux = aux->Fesq;
    }
    // Verificando que lado se o nó é filho direito ou esquerdo
    if (ant->chave <= valor){
        ant->Fdir = n;
    }else{
        ant->Fesq = n;
    }
    n->pai = ant;
    // Chamando o balanceamento
    balanceamentoInsercao(arv, n, count);
    arv->numElementos++;
    return 1;
}

int remover(arv *arv, int valor, long int *count) {
    no *aux = arv->sentinela->Fdir, *sucessor, *x;
    char corOriginal;

    while (aux != arv->nulo && aux->chave != valor) {
        // Achar o nó a remover
        if (valor < aux->chave) {
            aux = aux->Fesq;
        } else {
            aux = aux->Fdir;
        }
    }

    if (aux == arv->nulo)
        // Chave não está na árvore
        return 0;

    arv->numElementos--;
    // Cor original para saber se sucessor será vermelho petro ou duplo preto
    corOriginal = aux->cor;

    if (aux->Fesq == arv->nulo) {
        // Não tem filho à esquerda
        x = aux->Fdir;
        if (aux == aux->pai->Fesq) {
            aux->pai->Fesq = aux->Fdir;
        } else {
            aux->pai->Fdir = aux->Fdir;
        }
        // Atualizar o pai do filho a direita. Mesmo que seja nulo, é importante para conseguir achar o pai e o irmão
        // do nó removido
        aux->Fdir->pai = aux->pai;
    } else if (aux->Fdir == arv->nulo) {
        // Não tem filho à direita
        x = aux->Fesq;
        if (aux == aux->pai->Fesq) {
            aux->pai->Fesq = aux->Fesq;
        } else {
            aux->pai->Fdir = aux->Fesq;
        }
        // Atualizar o pai do filho a esquerda.
        aux->Fesq->pai = aux->pai;
    } else {
        // Tem os dois filhos
        sucessor = aux->Fdir;
        // Achar o sucessor (nó mais à esquerda do filho à direita)
        while (sucessor->Fesq != arv->nulo) {
            sucessor = sucessor->Fesq;
        }
        corOriginal = sucessor->cor;
        x = sucessor->Fdir;

        // Colocar o sucessor no lugar de aux
        if (sucessor->pai == aux) {
            // Caso sucessor->Fdir seja nulo, atualizar o pai do sentinela nulo
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
        // Se o nó que ficou no lugar do nó removido é preto, realizar o balanceamento
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
    // Verificar se está desbalanceado (nó vermelho com filho vermelho)
    while (n->pai->cor == 'V') {
        if (n->pai == n->pai->pai->Fesq) {
            // Nó está a esquerda do avô
            tio = n->pai->pai->Fdir;
            if (tio->cor == 'V') {
                // Caso #1: Atualizar as cores e continuar a verificação a partir do avô
                n->pai->cor = 'P';
                tio->cor = 'P';
                n->pai->pai->cor = 'V';
                n = n->pai->pai;
            } else {
                // Tio é preto
                (*count)++;
                if (n == n->pai->Fdir) {
                    // Caso #2: Rotação Simples e continuar a partir do pai de "n", ir ao caso #3
                    n = n->pai;
                    rotacaoesq(arv, n);
                }
                // Caso #3: Atualização das cores e rotação a direita
                n->pai->cor = 'P';
                n->pai->pai->cor = 'V';
                rotacaodir(arv, n->pai->pai);
            }
        } else {
            // Nó está a direita do avô
            tio = n->pai->pai->Fesq;
            if (tio->cor == 'V') {
                // Caso #1: Atualizar as cores e continuar a verificação a partir do avô
                n->pai->cor = 'P';
                tio->cor = 'P';
                n->pai->pai->cor = 'V';
                n = n->pai->pai;
            } else {
                (*count)++;
                if (n == n->pai->Fesq) {
                    // Caso #2: Rotação dupla e continuar a partir do pai de "n", ir ao caso #3
                    n = n->pai;
                    rotacaodir(arv, n);
                }
                // Caso #3: Atualização das cores e rotação a esquerda
                n->pai->cor = 'P';
                n->pai->pai->cor = 'V';
                rotacaoesq(arv, n->pai->pai);
            }
        }
    }
    // Colorir a raiz de preto
    arv->sentinela->Fdir->cor = 'P';
}

void balanceamentoRemocao(arv *arv, no *n, long int *count) {
    no *irmao;
    // Enquanto "n" for duplo preto ou n não é a raiz
    while (n != arv->sentinela->Fdir && n->cor == 'P') {
        if (n == n->pai->Fesq) {
            // "n" é filho à esquerda
            irmao = n->pai->Fdir;
            if (irmao->cor == 'V') {
                // Caso #1: Rotação a esquerda e pode levar aos casos 2, 3 ou 4
                (*count)++;
                irmao->cor = 'P';
                n->pai->cor = 'V';
                rotacaoesq(arv, n->pai);
                irmao = n->pai->Fdir;
            }
            if (irmao->Fesq->cor == 'P' && irmao->Fdir->cor == 'P') {
                // Caso #2: Irmão é preto e todos os filhos são pretos
                irmao->cor = 'V';
                n = n->pai;
            } else {
                (*count)++;
                if (irmao->Fdir->cor == 'P') {
                    // Caso #3: Rotação à direita e leva ao caso 4
                    irmao->Fesq->cor = 'P';
                    irmao->cor = 'V';
                    rotacaodir(arv, irmao);
                    irmao = n->pai->Fdir;
                }
                // Caso #4: Rotação à esquerda
                irmao->cor = n->pai->cor;
                n->pai->cor = 'P';
                irmao->Fdir->cor = 'P';
                rotacaoesq(arv, n->pai);
                n = arv->sentinela->Fdir;
            }
        } else {
            // "n" é filho à direita
            irmao = n->pai->Fesq;
            if (irmao->cor == 'V') {
                // Caso #1: Rotação a direita e pode levar aos casos 2, 3 ou 4
                (*count)++;
                irmao->cor = 'P';
                n->pai->cor = 'V';
                rotacaodir(arv, n->pai);
                irmao = n->pai->Fesq;
            }
            if (irmao->Fesq->cor == 'P' && irmao->Fdir->cor == 'P') {
                // Caso #2: Irmão é preto e todos os filhos são pretos
                irmao->cor = 'V';
                n = n->pai;
            } else {
                (*count)++;
                if (irmao->Fesq->cor == 'P') {
                    // Caso #3: Rotação à esquerda e leva ao caso 4
                    irmao->Fdir->cor = 'P';
                    irmao->cor = 'V';
                    rotacaoesq(arv, irmao);
                    irmao = n->pai->Fesq;
                }
                // Caso #4: Rotação à direita
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