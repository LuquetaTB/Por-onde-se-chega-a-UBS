#include <stdio.h>
#include <stdlib.h>

typedef struct Aresta{
    No *destino;
    int minutos;
    int alagada;
    struct Aresta *prox_aresta;
}Aresta;

typedef struct No{
    char ponto[10];
    struct No *proximo;
    int eh_ubs;
    Aresta *adjacentes;
}No;

typedef struct Grafo{
    No *inicio;
    int quantidade;
}Grafo;

Grafo* criarGrafo(){
    Grafo *grafo = malloc(sizeof(Grafo));

    grafo->inicio = NULL;
    grafo->quantidade = 0;

    return grafo;
}

int main(){

    return 0;
}