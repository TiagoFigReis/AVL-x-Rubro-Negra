//
// Created by tiago on 07/06/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "Menu.h"
#include "iniciarteste.h"

void menu()
{
    int cod, n, *tam1, *tam2;
    printf("Digite a quantidade de testes que serao feitos:");
    scanf("%d", &n);
    tam1 = (int *)malloc(n * sizeof(int));
    tam2 = (int *)malloc(n * sizeof(int));
    printf("\nDigite a quantidade de elementos que voce quer inserir em cada teste:");
    for (int i = 0; i < n; i++)
        scanf("%d", &tam1[i]);
    printf("\nDigite a quantidade de elementos que voce quer buscar em cada teste:");
    for (int i = 0; i < n; i++)
        scanf("%d", &tam2[i]);
    do
    {
        printf("\nSelecione uma arvore:\n");
        printf("0 - Sair\n");
        printf("1 - AVL\n");
        printf("2 - Rubro Negra\n");
        scanf("%d", &cod);
        switch (cod)
        {
        case 0:
            break;
        default:
            iniciarTestes(tam1, tam2, n, cod);
        }
    } while (cod != 0);
    free(tam1);
    free(tam2);
}