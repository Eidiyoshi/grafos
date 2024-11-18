#include <stdio.h>
#include <stdlib.h>
#include "pilha.c"
#include "fila.c"
#include <limits.h>

typedef struct No {
    int v;
    int d;
    struct No *prox;
} No;

typedef No *p_no;

typedef struct {
    int n;
    p_no *adjacencia;

} Grafo;

typedef Grafo *p_grafo;

p_grafo criar_grafo(int n) {
    int i;
    p_grafo g = malloc(sizeof(Grafo));
    g->n = n;
    g->adjacencia = malloc(n * sizeof(p_no));
    for (i = 0; i < n; i++)
        g->adjacencia[i] = NULL;
    return g;
}

void libera_lista(p_no lista) {
    if (lista != NULL) {
        libera_lista(lista->prox);
        free(lista);
    }
}

void destroi_grafo(p_grafo g) {
    int i;
    for (i = 0; i < g->n; i++)
        libera_lista(g->adjacencia[i]);
    free(g->adjacencia);
    free(g);
}

p_no insere_na_lista(p_no lista, int v, int d) {
    p_no novo = malloc(sizeof(No));
    novo->v = v;
    novo->d = d;
    novo->prox = lista;
    return novo;
}

void insere_aresta(p_grafo g, int u, int v, int d) {
    g->adjacencia[v] = insere_na_lista(g->adjacencia[v], u, d);
    g->adjacencia[u] = insere_na_lista(g->adjacencia[u], v, d);
}

p_no remove_da_lista(p_no lista, int v) {
    p_no proximo;
    if (lista == NULL)
        return NULL;
    else if (lista->v == v) {
        proximo = lista->prox;
        free(lista);
        return proximo;
    } else {
        lista->prox = remove_da_lista(lista->prox, v);
        return lista;
    }
}

void remove_aresta(p_grafo g, int u, int v) {
    g->adjacencia[u] = remove_da_lista(g->adjacencia[u], v);
    g->adjacencia[v] = remove_da_lista(g->adjacencia[v], u);
}

int tem_aresta(p_grafo g, int u, int v) {
    p_no t;
    for (t = g->adjacencia[u]; t != NULL; t = t->prox)
        if (t->v == v)
            return 1;
    return 0;
}

int * busca_em_profundidade(p_grafo g, int s) {
    int v;
    int *pai = malloc(g->n * sizeof(int));
    int *visitado = malloc(g->n * sizeof(int));
    p_pilha p = criarPilha();
    p_no listaAdjacente;
    for (v = 0; v < g->n; v++) {
        pai[v] = -1;
        visitado[v] = 0;
    }
    empilhar(p,s);
    pai[s] = s;
    while(!pilhaVazia(p)) {
        v = desempilhar(p);
        visitado[v] = 1;
        for(listaAdjacente = g->adjacencia[v] ; listaAdjacente != NULL; listaAdjacente = listaAdjacente->prox ){
            if(!visitado[listaAdjacente->v]){
                visitado[listaAdjacente->v] = 1;
                pai[listaAdjacente->v] = v;
                empilhar(p,listaAdjacente->v);
            }
        }
    }

    destroiPilha(p);
    free(visitado);
    return pai;
}

int * busca_em_largura(p_grafo g, int s) {
    int v;
    int *pai = malloc(g->n * sizeof(int));
    int *visitado = malloc(g->n * sizeof(int));
    p_fila f = criarFila();
    p_no listaAdjacente;
    for (v = 0; v < g->n; v++) {
        pai[v] = -1;
        visitado[v] = 0;
    }
    enfileira(f,s,0);
    pai[s] = s;
    visitado[s] = 1;
    while(!filaVazia(f)) {
        v = desenfileira(f);
        for(listaAdjacente = g->adjacencia[v] ; listaAdjacente != NULL; listaAdjacente = listaAdjacente->prox ){
            if(!visitado[listaAdjacente->v]){
                visitado[listaAdjacente->v] = 1;
                pai[listaAdjacente->v] = v;
                enfileira(f,listaAdjacente->v, 0);
            }
        }
    }
    destroiFila(f);
    free(visitado);
    return pai;
}

void enfileiraPrioridade(p_fila f, int n, int indexAtual, int distanciaAtual){
    enfileira(f, indexAtual, distanciaAtual);
    for(int i = 1; i < n ; i++){
            int distancia = f->filaDistancia[i];
            int index = f->fila[i];
            int j = i - 1;

            while( j >= 0 && f->filaDistancia[j] > distancia){
                f->filaDistancia[j + 1] = f->filaDistancia[j];
                f->fila[j + 1] = f->fila[j];
                j--;
            }
            f->filaDistancia[j + 1] = distancia;
            f->fila[j + 1] = index;
    }
}


void dijsktra(p_grafo g, int origem, int alvo){
    int *distancia = malloc(g->n * sizeof(int));
    int *visitado = malloc(g->n * sizeof(int));
    int *pai = malloc(g->n * sizeof(int));
    int v;
    p_no listaAdjacente;
    p_fila fPrioridade = criarFila();
    for (int w = 0; w < g->n; w++) {
        pai[w] = -1;
        distancia[w] = INT_MAX;
        visitado[w] = 0;
    }
    distancia[origem] = 0;
    visitado[origem] = 1;

    enfileira(fPrioridade, origem, 0);
    while(!filaVazia(fPrioridade)){
        v = desenfileira(fPrioridade);
        printf("%d, ",v);
        visitado[v] = 1;
        if(v == alvo){
            printf("Alvo encontrado, distancia de %d\n", distancia[v]);
            printf("Caminho de encontro");
            for(int i = 0; i < g->n; i++){
                printf("%d, ",pai[i]);
            }
        }

        for(listaAdjacente = g->adjacencia[v]; listaAdjacente != NULL; listaAdjacente = listaAdjacente->prox){ // considerando todos os possiveis lugares pra ir
            if(distancia[v] + listaAdjacente->d < distancia[listaAdjacente->v]){
                distancia[listaAdjacente->v] = distancia[v] + listaAdjacente->d;
                pai[listaAdjacente->v] = v;
                enfileiraPrioridade(fPrioridade, g->n, listaAdjacente->v, distancia[listaAdjacente->v]);
                visitado[listaAdjacente->v] = 1;
            }

        }

    }

}

void main(){
    int n = 6;
    p_grafo grafo = criar_grafo(n);

    insere_aresta(grafo, 0, 1, 2);
    insere_aresta(grafo, 0, 2, 1);

    insere_aresta(grafo, 1, 3, 1);
    insere_aresta(grafo, 2, 4, 1);

    insere_aresta(grafo, 3, 5, 2);
    insere_aresta(grafo, 4, 5, 4);

    dijsktra(grafo, 0, 5);

    int *lista = busca_em_profundidade(grafo, 0);

    return;
}
