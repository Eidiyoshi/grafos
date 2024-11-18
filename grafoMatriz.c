#include <stdio.h>
#include <stdlib.h>
#include "pilha.c"
#include "fila.c"

typedef struct {
    int **adj;
    int n;
} Grafo;

typedef Grafo *p_grafo;

p_grafo criar_grafo(int n) {
    int i, j;
    p_grafo g = malloc(sizeof(Grafo));
    g->n = n;
    g->adj = malloc(n * sizeof(int *));
    for (i = 0; i < n; i++)
        g->adj[i] = malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            g->adj[i][j] = 0;
    return g;
}

void destroi_grafo(p_grafo g) {
    int i;
    for (i = 0; i < g->n; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
}

void insere_aresta(p_grafo g, int u, int v, int d) {
    g->adj[u][v] = d;
    g->adj[v][u] = d;
}

void remove_aresta(p_grafo g, int u, int v) {
    g->adj[u][v] = 0;
    g->adj[v][u] = 0;
}

int tem_aresta(p_grafo g, int u, int v) {
    return g->adj[u][v];
}


int * busca_em_profundidade(p_grafo g, int s) {
    int w, v;
    int *pai = malloc(g->n * sizeof(int));
    int *visitado = malloc(g->n * sizeof(int));
    p_pilha p = criarPilha();
    for (v = 0; v < g->n; v++) {
        pai[v] = -1;
        visitado[v] = 0;
    }
    empilhar(p,s);
    pai[s] = s;
    while(!pilhaVazia(p)) {
        v = desempilhar(p);
        visitado[v] = 1;
        for (w = 0; w < g->n; w++)
            if (g->adj[v][w] && !visitado[w]) {
                pai[w] = v;
                empilhar(p, w);
                }
        }
        destroiPilha(p);
        free(visitado);
        return pai;
}

int * busca_em_largura(p_grafo g, int s) {
    int w, v;
    int *pai = malloc(g->n * sizeof(int));
    int *visitado = malloc(g->n * sizeof(int));
    p_fila f = criarFila();
    for (v = 0; v < g->n; v++) {
        pai[v] = -1;
        visitado[v] = 0;
    }
    enfileira(f,s);
    pai[s] = s;
    visitado[s] = 1;
    while(!filaVazia(f)) {
        v = desenfileira(f);
        for (w = 0; w < g->n; w++)
            if (g->adj[v][w] && !visitado[w]) {
                visitado[w] = 1;/*evita repetição na fila*/
                pai[w] = v;
                enfileira(f, w, 0);
            }
    }
    destroiFila(f);
    free(visitado);
    return pai;
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

    int *lista = busca_em_profundidade(grafo, 0);
    for(int i = 0; i < n; i++){
        printf("%d, ",lista[i]);
    }
    printf("\n");

    return;
}
