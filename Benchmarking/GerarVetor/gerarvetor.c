#include <stdio.h>
#include <stdlib.h>
#include "gerarvetor.h"


int* gerarVetorCrescente (int n)
{
    int *v = (int *) malloc(n * sizeof(int));
    if (!v)
    {
        printf("Erro ao alocar memória para o vetor.\n");
        return NULL;
    }
    for (int i = 0; i < n; i++)
    {
        v[i] = i;
    }
    return v;
}


int* gerarVetorAleatorio (int n, int seed)
{
    // Alocando um vetor de tamanho "n"
    int *v = (int *) malloc(n * sizeof(int));
    if (!v)
    {
        printf("Erro ao alocar memória para o vetor.\n");
        return NULL;
    }
    srand(seed);
    for (int i = 0; i < n; i++)
    {
        v[i] = rand();
    }
    return v;
}