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
    // Inicialização das variáveis, sentinela->Fdir é a raiz da árvore
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
    // Inicialização das variáveis
    novo_no->chave = valor;
    novo_no->Fdir = novo_no->Fesq = NULL;
    novo_no->pai = arv->sentinela;
    novo_no->fb = 0;
    arv->numElementos++;

    if (!arv->sentinela->Fdir)
    {
        // Não há raiz, então o nó a inserir é a raiz da árvore
        arv->sentinela->Fdir = novo_no;
        return 1;
    }

    aux = arv->sentinela->Fdir;
    // Achando o lugar que deve ser inserido o novo nó
    while (aux)
    {
        // É importante manter quem é o pai do nó a inserir, então novo_no->pai é atualizado a cada iteração para
        // guardar o pai de aux
        novo_no->pai = aux;
        if (aux->chave > valor)
        {
            aux = aux->Fesq;
        } else {
            aux = aux->Fdir;
        }
    }

    // Verificando se o novo_no é filho a esquerda ou a direita
    if (novo_no->pai->chave > valor)
    {
        novo_no->pai->Fesq = novo_no;
    } else {
        novo_no->pai->Fdir = novo_no;
    }

    // Atualizar o fator de balanceamento
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
    // Encontrando o nó com o valor a remover
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
        // Atualizar o fator de balanceamento
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
        // Atualizar o fator de balanceamento
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
        // Atualizar o fator de balanceamento
        atualizaFB_remocao(arv, aux->pai, valor, count);
        free(aux);
        return 1;
    }
    // Possui os dois filhos
    // Procurar o sucessor (nó mais a esquerda do filho a direita)
    sucessor = aux->Fdir;
    while (sucessor->Fesq)
    {
        sucessor = sucessor->Fesq;
    }
    // Copiando a chave do sucessor para o nó removido
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
    // Atualizar o fb considerando o sucessor como o nó removido
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
        // Atualizar o fator de balanceamento considerando de qual lado foi inserido o nó
        // Se foi a esquerda então subtrai 1, soma 1 caso contrário
        if (aux->pai->chave > novoNo->chave)
        {
            aux->pai->fb--;
        } else {
            aux->pai->fb++;
        }
        aux = aux->pai;
        // Condição de parada se o nó desbalanceou, fb = -2 ou 2, se o fb é igual a 0, sem alteração dos nós acima
        // desse nó com fb = 0, ou se atingiu a raiz.
    }  while ((aux->fb == 1 || aux->fb == -1) && aux != arv->sentinela->Fdir);

    if (aux->fb == 2 || aux->fb == -2)
    {
        // Se desbalanceou, realiza o balanceamento
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
        // Desbalanceamento à direita
        filho = noDesbal->Fdir;
        if (filho->fb == -1)
        {
            (*count)++;
            neto = filho->Fesq;
            fbNeto = neto->fb;
            neto->fb = 0;
            // Rotação dupla
            rotacaoDir(filho);
            rotacaoEsq(noDesbal);
            // Atualizando os fbs dos nós envolvidos na rotação
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
            // Rotação Simples
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
        // fb == -2 (Desbalanceamento à esquerda)
        filho = noDesbal->Fesq;
        if (filho->fb == 1)
        {
            (*count)++;
            neto = filho->Fdir;
            fbNeto = neto->fb;
            neto->fb = 0;
            // Rotação Dupla
            rotacaoEsq(filho);
            rotacaoDir(noDesbal);
            // Atualizando os fbs dos nós envolvidos na rotação
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
            // Rotação Simples
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
    // Atualizar o fb do pai
    if (pai->chave > chaveRemovida) {
        pai->fb++;
    } else {
        pai->fb--;
    }
    // Sair do while se o fb desbalanceou (fb = 2 ou -2), se fb = 1 ou -1, sem necessidade de continuar a atualização
    // do fb ou se atingiu a raiz
    while (aux->fb == 0 && aux != arv->sentinela->Fdir)
    {
        aux = aux->pai;
        if (aux->chave > chaveRemovida) {
            // Nó removido a esquerda, somar 1 ao fb
            aux->fb++;
        } else {
            // Nó removido a direita, subtrair 1 ao fb
            aux->fb--;
        }
    }

    if (aux->fb == 2 || aux->fb == -2)
    {
        // Nó desbalanceado, realizar o balanceamento
        balanceamento(arv, aux, count);
        if ((aux->pai != arv->sentinela) && (aux->pai->fb == 0))
        {
            // Se não atingiu nenhuma das condições de parada especificadas anteriormente, continuar a atualizar o fb
            atualizaFB_remocao(arv, aux->pai->pai, chaveRemovida, count);
        }
    }

    return;
}