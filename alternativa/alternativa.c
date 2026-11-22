#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Representa a estrutura de uma aresta/trecho
typedef struct Aresta{
    No *destino;
    int minutos;
    int alagada;
    struct Aresta *prox_aresta;
}Aresta;

//Representa a estrutura de um nó/vertice
typedef struct No{
    char ponto[10];
    struct No *proximo;
    int eh_ubs;
    Aresta *adjacentes;
}No;

// Estrutura para representar o grafo com pontero para o nó inicial e a quantidade de nós
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
No* buscar_ponto(Grafo *grafo, const char *codigo){
    No *atual = grafo->inicio;

    while(atual != NULL){
        if(strcmp(atual->ponto, codigo)==0){
            return atual;
        }

        atual = atual->proximo;
    }

    return NULL;
}
void adicionar_ponto(Grafo *grafo, const char *codigo){
    if(strlen(codigo) > 9){
        printf("ERRO\n");
        return;
    }

    No *resultado = buscar_ponto(grafo, codigo);

    if(resultado==NULL){
        No *novo = malloc(sizeof(No));
        if (novo == NULL){
            printf("ERRO\n");
            return;
        }

        strcpy(novo->ponto, codigo);
        novo->eh_ubs = 0;
        novo->adjacentes = NULL;

        novo->proximo = grafo->inicio;
        grafo->inicio = novo;

        grafo->quantidade++;

        printf("OK\n");
    }else{
        printf("DUPLICADO\n");
    }
}
int main(){

    return 0;
}