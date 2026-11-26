#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define TAM_CODIGO 12

typedef struct No No;
//Representa a estrutura de uma aresta/trecho
typedef struct Aresta{
    No *destino;
    int minutos;
    int alagada;
    struct Aresta *prox_aresta;
}Aresta;

//Representa a estrutura de um nó/vertice
typedef struct No{
    char ponto[13];
    struct No *proximo;
    int eh_ubs;
    int visitado;
    int passos;
    int tempominutos;
    struct No *anterior;
    Aresta *adjacentes;
} No;

// Estrutura para representar o grafo com pontero para o nó inicial e a quantidade de nós
typedef struct Grafo{
    No *inicio;
    int quantidade;
}Grafo;

typedef struct Nofila{
    No *ponto;
    struct Nofila *prox;
}Nofila;

typedef struct Fila{
    Nofila *inicio;
    Nofila *fim;
}Fila;

Grafo* criarGrafo(){
    Grafo *grafo = malloc(sizeof(Grafo));

    grafo->inicio = NULL;
    grafo->quantidade = 0;

    return grafo;
}

// Compara dois códigos caractere por caractere. Retorna <0 se a vem antes, 0 se iguais, >0 se a vem depois.
int comparar_codigos(const char *a, const char *b){
    int i = 0;
    while(a[i] != '\0' && a[i] == b[i]){
        i++;
    }
    return (unsigned char)a[i] - (unsigned char)b[i];
}

// Copia o código para o destino (no máximo TAM_CODIGO caracteres).
void copiar_codigo(char *destino, const char *origem){
    int i = 0;
    while(origem[i] != '\0' && i < TAM_CODIGO){
        destino[i] = origem[i];
        i++;
    }
    destino[i] = '\0';
}

No* buscar_ponto(Grafo *grafo, const char *codigo){
    No *atual = grafo->inicio;

    while(atual != NULL){
        if(comparar_codigos(atual->ponto, codigo)==0){
            return atual;
        }

        atual = atual->proximo;
    }

    return NULL;
}
void adicionar_ponto(Grafo *grafo, const char *codigo, int eh_ubs){
    No *resultado = buscar_ponto(grafo, codigo);

    if(resultado==NULL){
        No *novo = malloc(sizeof(No));
        if (novo == NULL){
            fprintf(stderr, "Sem memoria\n");
            exit(1);
        }

        copiar_codigo(novo->ponto, codigo);
        novo->eh_ubs = eh_ubs;
        novo->visitado = 0;
        novo->passos = 0;
        novo->tempominutos = 0;
        novo->anterior = NULL;
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
    No *ponto_a = buscar_ponto(grafo, a);
    No *ponto_b = buscar_ponto(grafo, b);

    if(ponto_a == NULL || ponto_b == NULL){
        printf("NAOEXISTE\n");
        return;
    }

    if(minutos <= 0 || ponto_a == ponto_b){
        printf("INVALIDO\n");
        return;
    }

    if(buscar_trecho(ponto_a, ponto_b)){
        printf("DUPLICADO\n");
        return;
    }

    Aresta *aresta_ab = malloc(sizeof(Aresta));
    Aresta *aresta_ba = malloc(sizeof(Aresta));
    if(aresta_ab == NULL || aresta_ba == NULL){
        fprintf(stderr, "Sem memoria\n");
        exit(1);
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
void inicializarFila(Fila *fila){
    fila->inicio = NULL;
    fila->fim = NULL;
}
int estavazia(Fila *fila){
    return fila->inicio == NULL;
}
void enfileirar(Fila *fila, No *no){
    Nofila *novo = malloc(sizeof(Nofila));
    if(novo == NULL){
        fprintf(stderr, "Sem memoria\n");
        exit(1);
    }

    novo->ponto = no;
    novo->prox = NULL;

    if(estavazia(fila)){
        fila->inicio = novo;
        fila->fim = novo;
    }else{
        fila->fim->prox = novo;
        fila->fim = novo;
    }

}
No* desenfileirar(Fila *fila){
    if(estavazia(fila)){
        return NULL;
    }
    Nofila *aux = fila->inicio;
    No *ponto = aux->ponto;
    fila->inicio = fila->inicio->prox;
    
    if(fila->inicio == NULL){
        fila->fim = NULL;
    }

    free(aux);
    return ponto;
}

//Esvazia a fila liberando a memória dos nós que sobraram
void liberar_fila(Fila *fila){
    while(!estavazia(fila)){
        desenfileirar(fila);
    }
}

void inicializar_visitados(Grafo *grafo){
    No *atual = grafo->inicio;

    while(atual != NULL){
        atual->visitado = 0;
        atual = atual->proximo;
    }

}

//BFS: menor número de trechos de a até b. Retorna -1 se não houver caminho.
int qnt_passos(Grafo *grafo, Fila *fila, No *ponto_a, No *ponto_b){
    if(ponto_a == ponto_b){
        return 0;
    }

    inicializar_visitados(grafo);
    inicializarFila(fila);

    ponto_a->passos = 0;
    ponto_a->visitado = 1;
    enfileirar(fila, ponto_a);

    while (!estavazia(fila)){
        No *vertice = desenfileirar(fila);
        Aresta *vizinhos = vertice->adjacentes;

        while(vizinhos != NULL){
            if(vizinhos->alagada || vizinhos->destino->visitado){
                vizinhos = vizinhos->prox_aresta;
                continue;
            }
            vizinhos->destino->visitado = 1;
            vizinhos->destino->passos = vertice->passos + 1;

            if(vizinhos->destino == ponto_b){
                liberar_fila(fila);
                return vizinhos->destino->passos;
            }else{
                enfileirar(fila, vizinhos->destino);
            }

            vizinhos = vizinhos->prox_aresta;
        }
    }

    return -1;
}

//Trata a operação PASSOS a b
void passos(Grafo *grafo, Fila *fila, const char *codA, const char *codB){
    No *ponto_a = buscar_ponto(grafo, codA);
    No *ponto_b = buscar_ponto(grafo, codB);

    if(ponto_a == NULL || ponto_b == NULL){
        printf("NAOEXISTE\n");
        return;
    }

    int resultado = qnt_passos(grafo, fila, ponto_a, ponto_b);

    if(resultado == -1){
        printf("SEMCAMINHO\n");
    }else{
        printf("%d\n", resultado);
    }
}

//Função auxiliar para achar o nó não visitado com menor tempo em minutos
No* menor_tempo(Grafo *grafo){
    No *atual = grafo->inicio;
    No *menor = NULL;
    int menorValor = INT_MAX;

    while(atual != NULL){
        if(!atual->visitado && atual->tempominutos < menorValor){
            menorValor = atual->tempominutos;
            menor = atual;
        }
        atual = atual->proximo;
    }
    return menor;
}

//Função que executa o Dijkstra a partir da origem até encontrar o destino, com o menor tempo em minutos, considerando os trechos alagados como intransitáveis
int executaDijkstra(Grafo *grafo, No *origem, No *destino){
    No *atual = grafo->inicio;
    while(atual != NULL){
        atual->tempominutos = INT_MAX;
        atual->anterior = NULL;
        atual->visitado = 0;
        atual = atual->proximo;
    }

    origem->tempominutos = 0;

    while(1){
        No *noAtual = menor_tempo(grafo);

        if(noAtual == NULL || noAtual == destino){
            break;
        }
        noAtual->visitado = 1;

        Aresta *aresta = noAtual->adjacentes;
        while(aresta != NULL){
            if(!aresta->alagada && !aresta->destino->visitado){
                int novoTempo = noAtual->tempominutos + aresta->minutos;
                if(novoTempo < aresta->destino->tempominutos){
                    aresta->destino->tempominutos = novoTempo;
                    aresta->destino->anterior = noAtual;
                }
            }
            aresta = aresta->prox_aresta;
        }
    }

    if(destino == NULL){
        return 0;
    }
    return (destino->tempominutos == INT_MAX) ? -1 : destino->tempominutos;
}

//Trata a operação TEMPO a b
void tempo(Grafo *grafo, const char *codA, const char *codB){
    No *ponto_a = buscar_ponto(grafo, codA);
    No *ponto_b = buscar_ponto(grafo, codB);

    if(ponto_a == NULL || ponto_b == NULL){
        printf("NAOEXISTE\n");
        return;
    }

    int resultado = executaDijkstra(grafo, ponto_a, ponto_b);

    if(resultado == -1){
        printf("SEMCAMINHO\n");
    }else{
        printf("%d\n", resultado);
    }
}

//Função simples para imprrimir o caminho encontrado pelo Dijkstra, recursivamente, do destino até a origem
void imprimircaminhorecur(No *atual){
    if(atual == NULL){
        return;
    }

    imprimircaminhorecur(atual->anterior);
    printf(" %s", atual->ponto);
}

//Função que realiza a rota entre dois pontos, usando o Dijkstra para achar o menor tempo em minutos, considerando os trechos alagados como inacessíveis, e traz a tratativa dele.
//Caso algum ponto não exista, imprime "NAOEXISTE". Caso não exista caminho entre os pontos, imprime "SEMCAMINHO".
void rota(Grafo *grafo, const char *codA, const char *codB){
    No *ponto_a = buscar_ponto(grafo, codA);
    No *ponto_b = buscar_ponto(grafo, codB);

    if(ponto_a == NULL || ponto_b == NULL){
        printf("NAOEXISTE\n");
        return;
    }

    int tempo = executaDijkstra(grafo, ponto_a, ponto_b);

    if(tempo == -1){
        printf("SEMCAMINHO\n");
        return;
    }

    printf("%d", tempo);
    imprimircaminhorecur(ponto_b);
    printf("\n");
}

//Trata a operação UBSPROX a: roda o Dijkstra completo a partir de a e escolhe a UBS de menor tempo (empate: menor código)
void ubsprox(Grafo *grafo, const char *codA){
    No *ponto_a = buscar_ponto(grafo, codA);

    if(ponto_a == NULL){
        printf("NAOEXISTE\n");
        return;
    }

    executaDijkstra(grafo, ponto_a, NULL);

    No *melhor = NULL;
    No *atual = grafo->inicio;
    while(atual != NULL){
        if(atual->eh_ubs && atual->tempominutos != INT_MAX){
            if(melhor == NULL
               || atual->tempominutos < melhor->tempominutos
               || (atual->tempominutos == melhor->tempominutos
                   && comparar_codigos(atual->ponto, melhor->ponto) < 0)){
                melhor = atual;
            }
        }
        atual = atual->proximo;
    }

    if(melhor == NULL){
        printf("NENHUMA\n");
    }else{
        printf("%s %d\n", melhor->ponto, melhor->tempominutos);
    }
}

//Merge sort próprio para ordenar os pontos pelo código (o enunciado proíbe qsort no núcleo)
void intercalar(No **v, No **aux, int inicio, int meio, int fim){
    int i = inicio, j = meio, k = inicio;

    while(i < meio && j < fim){
        if(comparar_codigos(v[i]->ponto, v[j]->ponto) <= 0){
            aux[k++] = v[i++];
        }else{
            aux[k++] = v[j++];
        }
    }
    while(i < meio){
        aux[k++] = v[i++];
    }
    while(j < fim){
        aux[k++] = v[j++];
    }
    for(k = inicio; k < fim; k++){
        v[k] = aux[k];
    }
}
void ordenar_pontos(No **v, No **aux, int inicio, int fim){
    if(fim - inicio < 2){
        return;
    }
    int meio = (inicio + fim) / 2;
    ordenar_pontos(v, aux, inicio, meio);
    ordenar_pontos(v, aux, meio, fim);
    intercalar(v, aux, inicio, meio, fim);
}

int isolados(Grafo *grafo, Fila *fila){
    inicializar_visitados(grafo);
    inicializarFila(fila);

    int cont = 0;

    No *atual = grafo->inicio;
    while(atual != NULL){
        if(atual->eh_ubs){
            atual->visitado = 1;
            enfileirar(fila, atual);
        }

        atual = atual->proximo;
    }

    while(!estavazia(fila)){
        No *vertice = desenfileirar(fila);
        Aresta *vizinhos = vertice->adjacentes;

        while(vizinhos != NULL){
            if(vizinhos->alagada || vizinhos->destino->visitado){
                vizinhos = vizinhos->prox_aresta;
                continue;
            }

            vizinhos->destino->visitado = 1;
            enfileirar(fila, vizinhos->destino);

            vizinhos = vizinhos->prox_aresta;
        }   
    }

    atual = grafo->inicio;
    while (atual !=NULL){
        if(!atual->visitado){
            cont++;
        }
        atual = atual->proximo;
    }

    return cont;
    
}

//Trata a operação ISOLADOS: imprime k e depois os k códigos em ordem crescente
void imprimir_isolados(Grafo *grafo, Fila *fila){
    int k = isolados(grafo, fila);
    printf("%d\n", k);

    if(k == 0){
        return;
    }

    No **vetor = malloc(k * sizeof(No*));
    No **aux = malloc(k * sizeof(No*));
    if(vetor == NULL || aux == NULL){
        fprintf(stderr, "Sem memoria\n");
        exit(1);
    }

    int i = 0;
    No *atual = grafo->inicio;
    while(atual != NULL){
        if(!atual->visitado){
            vetor[i++] = atual;
        }
        atual = atual->proximo;
    }

    ordenar_pontos(vetor, aux, 0, k);

    for(i = 0; i < k; i++){
        printf("%s\n", vetor[i]->ponto);
    }

    free(vetor);
    free(aux);
}

// Lê o arquivo inteiro para a memória. Devolve um vetor de linhas e a quantidade em *total.
char** ler_arquivo(const char *caminho, int *total){
    FILE *arq = fopen(caminho, "r");
    if(arq == NULL){
        fprintf(stderr, "Nao foi possivel abrir %s\n", caminho);
        exit(1);
    }

    int capacidade = 1024;
    int n = 0;
    char **linhas = malloc(capacidade * sizeof(char*));
    char buffer[256];

    while(fgets(buffer, sizeof(buffer), arq) != NULL){
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if(n == capacidade){
            capacidade *= 2;
            linhas = realloc(linhas, capacidade * sizeof(char*));
        }
        linhas[n] = malloc(strlen(buffer) + 1);
        strcpy(linhas[n], buffer);
        n++;
    }

    fclose(arq);
    *total = n;
    return linhas;
}

// Executa uma linha do arquivo de operações
void executar_linha(Grafo *grafo, Fila *fila, const char *linha){
    char op[32], a[64], b[64];
    int minutos;

    int lidos = sscanf(linha, "%31s %63s %63s %d", op, a, b, &minutos);

    if(lidos <= 0 || op[0] == '#'){
        return; // linha em branco ou comentário
    }

    if(strcmp(op, "PONTO") == 0 && lidos >= 2){
        int eh_ubs = (lidos >= 3 && strcmp(b, "UBS") == 0);
        adicionar_ponto(grafo, a, eh_ubs);
    }else if(strcmp(op, "TRECHO") == 0 && lidos == 4){
        adicionar_trecho(grafo, a, b, minutos);
    }else if(strcmp(op, "ALAGA") == 0 && lidos >= 3){
        alagar_trecho(grafo, a, b);
    }else if(strcmp(op, "LIBERA") == 0 && lidos >= 3){
        libera_trecho(grafo, a, b);
    }else if(strcmp(op, "PASSOS") == 0 && lidos >= 3){
        passos(grafo, fila, a, b);
    }else if(strcmp(op, "TEMPO") == 0 && lidos >= 3){
        tempo(grafo, a, b);
    }else if(strcmp(op, "ROTA") == 0 && lidos >= 3){
        rota(grafo, a, b);
    }else if(strcmp(op, "UBSPROX") == 0 && lidos >= 2){
        ubsprox(grafo, a);
    }else if(strcmp(op, "ISOLADOS") == 0){
        imprimir_isolados(grafo, fila);
    }
}

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "Uso: %s arquivo_de_operacoes.txt\n", argv[0]);
        return 1;
    }

    int total = 0;
    char **linhas = ler_arquivo(argv[1], &total);

    Grafo *grafo = criarGrafo();
    Fila fila;
    inicializarFila(&fila);

    clock_t inicio = clock();

    for(int i = 0; i < total; i++){
        executar_linha(grafo, &fila, linhas[i]);
    }

    clock_t fim = clock();
    long tempo_ms = (long)((fim - inicio) * 1000.0 / CLOCKS_PER_SEC);
    fprintf(stderr, "TEMPO_MS %ld\n", tempo_ms);

    for(int i = 0; i < total; i++){
        free(linhas[i]);
    }
    free(linhas);

    return 0;
}
