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
int buscar_trecho(No *origem, No *destino){
    Aresta *atual = origem->adjacentes;

    while(atual != NULL){
        if(atual->destino == destino){
            return 1;
        }

        atual = atual->prox_aresta;
    }

    return 0;
}
void adicionar_trecho(Grafo *grafo, const char *a, const char *b, int minutos){
    if(minutos <= 0 || strcmp(a,b) == 0){
        printf("INVALIDO\n");
        return;
    }

    No *ponto_a = buscar_ponto(grafo, a);
    if(ponto_a==NULL){
        printf("NAOEXISTE\n");
        return;
    }

    No *ponto_b = buscar_ponto(grafo, b);
    if(ponto_b == NULL){
        printf("NAOEXISTE\n");
        return;
    }

    if(buscar_trecho(ponto_a, ponto_b)){
        printf("DUPLICADO\n");
        return;
    }

    Aresta *aresta_ab = malloc(sizeof(Aresta));
    if(aresta_ab == NULL){
        printf("ERRO\n");
        return;
    }
    Aresta *aresta_ba = malloc(sizeof(Aresta));
    if(aresta_ba == NULL){
        printf("ERRO\n");
        free(aresta_ab);
        return;
    }

    aresta_ab->destino = ponto_b;
    aresta_ab->minutos = minutos;
    aresta_ab->alagada = 0;

    aresta_ab->prox_aresta = ponto_a->adjacentes;
    ponto_a->adjacentes = aresta_ab;

    aresta_ba->destino = ponto_a;
    aresta_ba->minutos = minutos;
    aresta_ba->alagada = 0;

    aresta_ba->prox_aresta = ponto_b->adjacentes;
    ponto_b->adjacentes = aresta_ba;

    printf("OK\n");

}

//Função auxiliar que procura uma aresta direta entre destino e origem, retornando um ponteiro para a aresta caso exista, ou NULL caso contrário
Aresta* buscar_aresta(No *origem, No *destino){
    if (origem == NULL || destino == NULL){
        return NULL;
    }

    Aresta *atual = origem->adjacentes;
    while(atual != NULL){
        if (atual->destino == destino){
            return atual;
        }
        atual = atual->prox_aresta;
    }

    return NULL;
}

//Função que alaga um trecho entre dois pontos, confere se os pontos existem, se a aresta existe e se já está alagada
void alagar_trecho(Grafo *grafo, const char *codA, const char *codB){
    No *noA = buscar_ponto(grafo, codA);
    No *noB = buscar_ponto(grafo, codB);

    if (noA == NULL || noB == NULL){
        printf("NAOEXISTE\n");
        return;
    }

    Aresta *ida = buscar_aresta(noA, noB);
    Aresta *volta = buscar_aresta(noB, noA);

    if (ida == NULL || volta == NULL){
        printf("NAOEXISTE\n");
        return;
    }

    if (ida->alagada == 1 || volta->alagada == 1){
        printf("JA\n");
        return;
    }

    ida->alagada = 1;
    volta->alagada = 1;

    printf("OK\n");
}

//Função que libera um trecho entre dois pontos, confere se os pontos existem, se a aresta existe e se já está liberada
void libera_trecho(Grafo *grafo, const char *codA, const char *codB){
    No *noA = buscar_ponto(grafo, codA);
    No *noB = buscar_ponto(grafo, codB);

    if (noA == NULL || noB == NULL){
        printf("NAOEXISTE\n");
        return;
    }

    Aresta *ida = buscar_aresta(noA, noB);
    Aresta *volta = buscar_aresta(noB, noA);

    if (ida == NULL || volta == NULL){
        printf("NAOEXISTE\n");
        return;
    }

    if (ida->alagada == 0 || volta->alagada == 0){
        printf("JA\n");
        return;
    }

    ida->alagada = 0;
    volta->alagada = 0;

    printf("OK\n");
}

int main(){

    return 0;
}
