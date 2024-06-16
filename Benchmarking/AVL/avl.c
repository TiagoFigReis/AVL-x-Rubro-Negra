//
// Created by caio on 07/06/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

struct no {
    int chave;
    struct no *Fesq;
    struct no *Fdir;
    struct no *pai;
    int fb;
};

struct avl {
    struct no *sentinela;
    int numElementos;
};


avl *criaArvore()
{
    avl *arv;

    arv = (avl*)malloc(sizeof(avl));

    if (!arv)
    {
        return NULL;
    }

    arv->sentinela = (no*)malloc(sizeof(no));

    if (!arv->sentinela)
    {
        return NULL;
    }

    arv->sentinela->pai = arv->sentinela->Fdir = arv->sentinela->Fesq = NULL;
    arv->sentinela->chave = -1000;
    arv->numElementos = 0;
    arv->sentinela->fb = 0;

    return arv;
}


int insereNo(avl *arv, int valor, long int *count)
{
    no *novo_no = (no*)malloc(sizeof(no)), *aux;

    if (!novo_no)
    {
        return 0;
    }

    novo_no->chave = valor;
    novo_no->Fdir = novo_no->Fesq = NULL;
    novo_no->pai = arv->sentinela;
    novo_no->fb = 0;
    arv->numElementos++;

    if (!arv->sentinela->Fdir)
    {
        arv->sentinela->Fdir = novo_no;
        return 1;
    }

    aux = arv->sentinela->Fdir;
    while (aux)
    {
        novo_no->pai = aux;
        if (aux->chave > valor)
        {
            aux = aux->Fesq;
        } else {
            aux = aux->Fdir;
        }
    }

    if (novo_no->pai->chave > valor)
    {
        novo_no->pai->Fesq = novo_no;
    } else {
        novo_no->pai->Fdir = novo_no;
    }

    atualizaFB_insercao(arv, novo_no, count);
    return 1;
}


int removeNo(avl *arv, int valor, long int *count)
{
    no *aux, *sucessor;

    if (!arv->sentinela->Fdir)
    {
        // Árvore vazia
        return 0;
    }

    aux = arv->sentinela->Fdir;
    while (aux && aux->chave != valor)
    {
        if (aux->chave > valor)
        {
            aux = aux->Fesq;
        } else {
            aux = aux->Fdir;
        }
    }

    if (!aux)
    {
        // Elemento não encontrado
        return -1;
    }

    arv->numElementos--;

    if (!aux->Fdir && !aux->Fesq)
    {
        // Não possui os dois filhos
        if (aux->pai->chave > valor)
        {
            aux->pai->Fesq = NULL;
        } else {
            aux->pai->Fdir = NULL;
        }

        atualizaFB_remocao(arv, aux->pai, valor, count);
        free(aux);
        return 1;
    }

    if (!aux->Fdir && aux->Fesq)
    {
        // Apenas filho esquerdo
        if (aux->pai->chave > valor)
        {
            aux->pai->Fesq = aux->Fesq;
        } else {
            aux->pai->Fdir = aux->Fesq;
        }

        aux->Fesq->pai = aux->pai;

        atualizaFB_remocao(arv, aux->pai, valor, count);
        free(aux);
        return 1;
    }

    if (!aux->Fesq && aux->Fdir)
    {
        // Apenas filho direito
        if (aux->pai->chave > valor)
        {
            aux->pai->Fesq = aux->Fdir;
        } else {
            aux->pai->Fdir = aux->Fdir;
        }

        aux->Fdir->pai = aux->pai;

        atualizaFB_remocao(arv, aux->pai, valor, count);
        free(aux);
        return 1;
    }
    // Possui os dois filhos
    sucessor = aux->Fdir;
    while (sucessor->Fesq)
    {
        sucessor = sucessor->Fesq;
    }

    aux->chave = sucessor->chave;

    if (sucessor->pai == aux)
    {
        aux->Fdir = sucessor->Fdir;
        if (sucessor->Fdir)
        {
            sucessor->Fdir->pai = aux;
        }

    } else {
        sucessor->pai->Fesq = sucessor->Fdir;
        if (sucessor->Fdir)
        {
            sucessor->Fdir->pai = sucessor->pai;
        }
    }

    atualizaFB_remocao(arv, sucessor->pai, sucessor->chave,count);
    free(sucessor);

    return 1;
}

void imprimeOrdem(no *raiz)
{
    if (raiz)
    {
        imprimeOrdem(raiz->Fesq);
        printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
        imprimeOrdem(raiz->Fdir);
    }
}


void imprimePreOrdem(no *raiz)
{
    if (raiz)
    {
        printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
        imprimePreOrdem(raiz->Fesq);
        imprimePreOrdem(raiz->Fdir);
    }
}


no *getRaiz(avl *arv)
{
    return arv->sentinela->Fdir;
}


int getNumElementos(avl *arv)
{
    return arv->numElementos;
}

int BuscaAVL(avl *arv, int valor){
    no *aux;
    aux = arv->sentinela->Fdir;
    if(!aux)
        return -1;
    while(aux && aux->chave != valor){
        if(aux->chave<=valor)
            aux = aux->Fdir;
        else
            aux = aux->Fesq;
    }
    if(!aux)
        return 0;
    else
        return 1;
}

void atualizaFB_insercao(avl *arv, no *novoNo, long int *count)
{
    no *aux = novoNo;

    do {
        if (aux->pai->chave > novoNo->chave)
        {
            aux->pai->fb--;
        } else {
            aux->pai->fb++;
        }
        aux = aux->pai;
    }  while (aux->fb != 2 && aux->fb != -2 && aux != arv->sentinela->Fdir && aux->fb != 0);

    if (aux->fb == 2 || aux->fb == -2)
    {
        balanceamento(arv, aux, count);
    }

    return;
}


void balanceamento(avl *arv, no *noDesbal, long int *count)
{
    no *filho, *neto;
    int fbNeto;

    if (noDesbal->fb == 2)
    {
        filho = noDesbal->Fdir;
        if (filho->fb == -1)
        {
            (*count)++;
            neto = filho->Fesq;
            fbNeto = neto->fb;
            neto->fb = 0;
            rotacaoDir(filho);
            rotacaoEsq(noDesbal);
            if (fbNeto == -1)
            {
                filho->fb = 1;
                noDesbal->fb = 0;
            } else if (fbNeto == 1)
            {
                noDesbal->fb = -1;
                filho->fb = 0;
            } else {
                noDesbal->fb = filho->fb = 0;
            }
        } else {
            (*count)++;
            rotacaoEsq(noDesbal);
            if (filho->fb == 1)
            {
                filho->fb = noDesbal->fb = 0;
            } else {
                noDesbal->fb = 1;
                filho->fb = -1;
            }
        }
    } else {
        // fb == -2
        filho = noDesbal->Fesq;
        if (filho->fb == 1)
        {
            (*count)++;
            neto = filho->Fdir;
            fbNeto = neto->fb;
            neto->fb = 0;
            rotacaoEsq(filho);
            rotacaoDir(noDesbal);
            if (fbNeto == -1)
            {
                noDesbal->fb = 1;
                filho->fb = 0;
            } else if (fbNeto == 1) {
                filho->fb = -1;
                noDesbal->fb = 0;
            } else {
                filho->fb = noDesbal->fb = 0;
            }
        } else {
            (*count)++;
            rotacaoDir(noDesbal);
            if (filho->fb == -1)
            {
                noDesbal->fb = filho->fb = 0;
            } else {
                noDesbal->fb = -1;
                filho->fb = 1;
            }
        }
    }
    return;
}


void rotacaoDir(no *noDesbal)
{
    no *neto = noDesbal->Fesq->Fdir;

    if (noDesbal->pai->chave > noDesbal->chave)
    {
        noDesbal->pai->Fesq = noDesbal->Fesq;
    } else {
        noDesbal->pai->Fdir = noDesbal->Fesq;
    }

    noDesbal->Fesq->pai = noDesbal->pai;
    noDesbal->pai = noDesbal->Fesq;
    noDesbal->Fesq->Fdir = noDesbal;

    noDesbal->Fesq = neto;
    if (neto)
    {
        neto->pai = noDesbal;
    }

    return;
}


void rotacaoEsq(no *noDesbal)
{
    no *neto = noDesbal->Fdir->Fesq;

    if (noDesbal->pai->chave > noDesbal->chave)
    {
        noDesbal->pai->Fesq = noDesbal->Fdir;
    } else {
        noDesbal->pai->Fdir = noDesbal->Fdir;
    }

    noDesbal->Fdir->pai = noDesbal->pai;
    noDesbal->pai = noDesbal->Fdir;
    noDesbal->Fdir->Fesq = noDesbal;

    noDesbal->Fdir = neto;
    if (neto)
    {
        neto->pai = noDesbal;
    }

    return;
}



void atualizaFB_remocao(avl *arv, no *pai, int chaveRemovida, long int *count) {
    no *aux = pai;

    if (aux == arv->sentinela)
    {
        return;
    }

    if (pai->chave > chaveRemovida) {
        pai->fb++;
    } else {
        pai->fb--;
    }

    while (aux->fb == 0 && aux != arv->sentinela->Fdir)
    {
        aux = aux->pai;
        if (aux->chave > chaveRemovida) {
            aux->fb++;
        } else {
            aux->fb--;
        }
    }

    if (aux->fb == 2 || aux->fb == -2)
    {
        balanceamento(arv, aux, count);
        if ((aux->pai != arv->sentinela) && (aux->pai->fb == 0))
        {
            atualizaFB_remocao(arv, aux->pai->pai, chaveRemovida, count);
        }
    }

    return;
}