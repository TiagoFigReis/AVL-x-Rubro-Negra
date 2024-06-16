//
// Created by tiago on 07/06/2024.
//
#include "Menu/menu.h"
#include <stdio.h>

int main(){
    printf ("Este programa e um benchmarking para testar e comparar a insercao, remocao e busca das arvores"
            " AVL e Rubro Negra. E possivel escolher a quantidade de elementos que serao inseridos e buscados."
            " Todos os elementos inseridos serao removidos. Ao final, sera escrito em um arquivo .csv na pasta resultados"
            " o tempo e a quantidade de rotacoes realizadas em cada teste. E recomendado o uso de planilhas para"
            " a visualizacao dos resultados.\n\n");
    menu();
    return 0;
}