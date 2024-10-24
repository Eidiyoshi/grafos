#include <stdio.h>

typedef struct {
    int **adj;
    int n;
} p_grafo;


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

void insere_aresta(p_grafo g, int u, int v) {
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void remove_aresta(p_grafo g, int u, int v) {
    g->adj[u][v] = 0;
    g->adj[v][u] = 0;
}

int tem_aresta(p_grafo g, int u, int v) {
    return g->adj[u][v];
}


void main(){

}
