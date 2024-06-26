//
// Created by caio on 07/06/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "avl.h"
#include "Rubro.h"
#include "gerarvetor.h"
#include "iniciarteste.h"

struct metrica{
    long double tempo;
    long int rotacoes;
};


void iniciarTestes (int tamanhoInsercao[], int tamanhoBusca[], int n, int cod)
{
    int *v, countInsercao = 0, elemAleatorio, *vetorAleatorio;
    metricas *metricasInsercao = (metricas*)calloc(sizeof(metricas), n);
    metricas *metricasRemocao = (metricas*)calloc(sizeof(metricas), n);
    metricas *metricasBusca = (metricas*)calloc(sizeof(metricas), n);
    avl *avl = criaArvore();
    arv *rubro = criaArvoreRubro();
    FILE *arq;
    clock_t inicio, final;
    char diretorio[40];

    if (!avl)
    {
        printf ("Erro ao criar a árvore.\n");
        return;
    }

    if (!metricasInsercao || !metricasRemocao || !metricasBusca)
    {
        printf ("Erro ao alocar memória para as métricas.\n");
        return;
    }

    if (cod == 1)
    {
        strcpy(diretorio, "../Resultados/ResultadosAvl.csv");
    } else {
        strcpy(diretorio, "../Resultados/ResultadosRubro.csv");
    }


    arq = fopen(diretorio, "w+");

    if (!arq)
    {
        printf ("Erro ao abrir o arquivo.\n");
        return;
    }


    fprintf (arq, "Entrada Insercao-Tempo Insercao-Rotacoes Remocao-Tempo Remocao-Rotacao Entrada-Busca Busca-Tempo\n");

    if (cod == 1) {
        // Testes com a AVL
        for (int i = 0; i < n; i++) {
            v = gerarVetorCrescente(tamanhoInsercao[i]);

            if (!v) {
                printf("Erro ao alocar memória para o vetor.\n");
                fclose(arq);
                return;
            }

            countInsercao = 0; // variável para saber se a inserção ocorreu corretamente
            inicio = clock();
            for (int j = 0; j < tamanhoInsercao[i]; j++) {
                countInsercao += insereNo(avl, j, &(metricasInsercao[i].rotacoes));
            }
            final = clock();
            metricasInsercao[i].tempo = (((long double) (final - inicio) / (CLOCKS_PER_SEC)));

            if (countInsercao != tamanhoInsercao[i]) {
                printf("Houve um erro na inserção de elementos na AVL.\n");
                return;
            }
            // Gerando o vetor de busca com chaves aleatórias com a semente "i", para ser consistente nas duas árvores
            vetorAleatorio = gerarVetorAleatorio(tamanhoBusca[i], i);

            if (!vetorAleatorio)
            {
                printf ("Erro ao alocar o vetor.\n");
                return;
            }

            inicio = clock();
            for (int j = 0; j < tamanhoBusca[i]; j++) {
                BuscaAVL(avl, vetorAleatorio[j]);
            }
            final = clock();
            metricasBusca[i].tempo = (((long double) (final - inicio) / (CLOCKS_PER_SEC)));

            inicio = clock();
            for (int j = 0; j < tamanhoInsercao[i]; j++) {
                // Removendo todos os elementos inseridos
                removeNo(avl, j, &(metricasRemocao[i].rotacoes));
            }
            final = clock();
            metricasRemocao[i].tempo = (((long double) (final - inicio) / (CLOCKS_PER_SEC)));

            free(v);
            // Escrevendo no arquivo os resultados desse teste
            fprintf(arq, "%d %Lf %ld %Lf %ld %d %Lf\n", tamanhoInsercao[i], metricasInsercao[i].tempo,
                    metricasInsercao[i].rotacoes, metricasRemocao[i].tempo, metricasRemocao[i].rotacoes,
                    tamanhoBusca[i], metricasBusca[i].tempo);
        }
    } else {
        // Teste com a Rubro Negra
        for (int i = 0; i < n; i++) {
            v = gerarVetorCrescente(tamanhoInsercao[i]);

            if (!v) {
                printf("Erro ao alocar memória para o vetor.\n");
                fclose(arq);
                return;
            }

            countInsercao = 0; // variável para saber se todos os elementos foram inseridos corretamente
            inicio = clock();
            for (int j = 0; j < tamanhoInsercao[i]; j++) {
                countInsercao += inserir(rubro, j, &(metricasInsercao[i].rotacoes));
            }
            final = clock();
            metricasInsercao[i].tempo = (((long double) (final - inicio) / (CLOCKS_PER_SEC)));

            if (countInsercao != tamanhoInsercao[i]) {
                printf("Houve um erro na inserção de elementos na AVL.\n");
                return;
            }
            // Gerando o vetor de busca com chaves aleatórias com a semente "i", para ser consistente nas duas árvores
            vetorAleatorio = gerarVetorAleatorio(tamanhoBusca[i], i);

            if (!vetorAleatorio)
            {
                printf ("Erro ao alocar o vetor.\n");
                return;
            }

            inicio = clock();
            for (int j = 0; j < tamanhoBusca[i]; j++) {
                BuscaRubro(rubro, vetorAleatorio[j]);
            }
            final = clock();
            metricasBusca[i].tempo = (((long double) (final - inicio) / (CLOCKS_PER_SEC)));

            inicio = clock();
            for (int j = 0; j < tamanhoInsercao[i]; j++) {
                remover(rubro, j, &(metricasRemocao[i].rotacoes));
            }
            final = clock();
            metricasRemocao[i].tempo = (((long double) (final - inicio) / (CLOCKS_PER_SEC)));

            free(v);
            free(vetorAleatorio);
            fprintf(arq, "%d %Lf %ld %Lf %ld %d %Lf\n", tamanhoInsercao[i], metricasInsercao[i].tempo,
                    metricasInsercao[i].rotacoes, metricasRemocao[i].tempo, metricasRemocao[i].rotacoes,
                    tamanhoBusca[i], metricasBusca[i].tempo);
        }
    }

    free(metricasRemocao);
    free(metricasInsercao);
    free(metricasBusca);

    fclose(arq);
    return;
}