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

int distancia_aresta(p_grafo g, int u, int v) {
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
    enfileira(f,s,0);
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

void dijsktra(p_grafo g, int origem, int alvo, int apenasAlvo){
    int *distancia = malloc(g->n * sizeof(int));
    int *visitado = malloc(g->n * sizeof(int));
    int *pai = malloc(g->n * sizeof(int));
    int v, achado = 0;
    p_fila fPrioridade = criarFila();
    for (int w = 0; w < g->n; w++) {
        pai[w] = -1;
        distancia[w] = INT_MAX;
        visitado[w] = 0;
    }
    distancia[origem] = 0;
    visitado[origem] = 1;
    pai[origem] = origem;

    enfileira(fPrioridade, origem, 0);
    while(!filaVazia(fPrioridade)){
        v = desenfileira(fPrioridade);
        visitado[v] = 1;

        if( v == alvo ) achado = 1;

        for(int w = 0; w < g->n; w++){ // considerando todos os possiveis lugares pra ir
            if( g->adj[v][w] && distancia[v] + g->adj[v][w] <= distancia[w]){
                distancia[w] = distancia[v] + g->adj[v][w];
                pai[w] = v;
                enfileiraPrioridade(fPrioridade, g->n, w, distancia[g->adj[v][w]]);
                visitado[w] = 1;
            }
        }
    }

    if(apenasAlvo){
        if(achado){
                int s;
                printf("Alvo achado\n");
                printf("Distancia de %d\n",distancia[alvo]);
                printf("Caminho Inverso: ");
                s = v;
                while(s != origem){
                    s = pai[s];
                    printf("%d, ",s);
                }
                printf("\n\n");
        }
    }else{
        for(int i = 0; i < g->n; i++){
            printf("numero: %d\n",i);
            printf("pai: %d\n",pai[i]);
            printf("distancia: %d\n\n",distancia[i]);
        }
    }

}

void printarLista(int *lista, int n){
    for(int i = 0; i < n; i++){
        printf("\nnumero: %d\n",i);
        printf("pai: %d\n",lista[i]);
    }
}

void main(){
    printf("Bem vindo, trabalho ED2 grafos\n");
    printf("Cristian Eidi Yoshimura\n");
    printf("Grafos de Matriz\n");

    int n;
    printf("O numero de nos desejados: ");
    scanf("%d",&n);

    p_grafo grafo = criar_grafo(n);

    int primeiro,segundo,distancia;
    int* lista;

    int escolha;
    do{
        printf("\n\n");
        printf("1- Inserir aresta\n");
        printf("2- Remover aresta\n");
        printf("3- Printar array da busca de profundidade\n");
        printf("4- Printar array da busca de largura\n");
        printf("5- Printar dijisktra com alvo\n");
        printf("6- Printar arvore geradora minima\n");
        printf("7- Caso 1 de arestas para testes com n = 6\n");
        printf("8- Caso 2 de arestas para testes com n = 7\n");

        printf("99- Destruir grafo e sair do programa\n");

        printf("Sua escolha: ");
        scanf("%d", &escolha);
        switch(escolha){
            case 1:
                printf("Primeiro no: ");
                scanf("%d",&primeiro);
                printf("Segundo no: ");
                scanf("%d",&segundo);
                printf("Distancia entre os nos: ");
                scanf("%d",&distancia);
                if(primeiro < n && segundo < n && distancia > 0 && primeiro != segundo){
                    insere_aresta(grafo, primeiro, segundo, distancia);
                }else
                    printf("erro no input\n");
                break;

            case 2:
                printf("Primeiro no: ");
                scanf("%d",&primeiro);
                printf("Segundo no: ");
                scanf("%d",&segundo);
                if(primeiro < n && segundo < n){
                    remove_aresta(grafo, primeiro, segundo);
                }else
                    printf("erro no input\n");
                break;

            case 3:
                printf("No de origem: ");
                scanf("%d",&primeiro);
                if(primeiro < n){
                    lista = busca_em_profundidade(grafo, primeiro);
                printarLista(lista,n);
                }else
                    printf("erro no input\n");
                break;

            case 4:
                printf("No de origem: ");
                scanf("%d",&primeiro);
                if(primeiro < n){
                    lista = busca_em_largura(grafo, primeiro);
                    printarLista(lista,n);
                }else
                    printf("\nerro no input\n");
                break;

            case 5:
                printf("No de origem: ");
                scanf("%d",&primeiro);
                printf("No de alvo: ");
                scanf("%d",&segundo);
                if(primeiro < n && segundo < n){
                    printf("\n");
                    dijsktra(grafo, primeiro, segundo, 1);
                }else
                    printf("erro no input\n");
                break;

            case 6:
                printf("No de origem: ");
                scanf("%d",&primeiro);
                if(primeiro < n){
                    printf("\n");
                    dijsktra(grafo, primeiro, segundo, 0);
                }else
                    printf("erro no input\n");
                break;

            case 7:
                insere_aresta(grafo, 0, 1, 2);
                insere_aresta(grafo, 0, 2, 1);

                insere_aresta(grafo, 1, 3, 1);
                insere_aresta(grafo, 2, 4, 1);

                insere_aresta(grafo, 3, 5, 2);
                insere_aresta(grafo, 4, 5, 4);
                break;

            case 8:
                insere_aresta(grafo, 0, 1, 4);

                insere_aresta(grafo, 1, 2, 3);
                insere_aresta(grafo, 1, 4, 2);

                insere_aresta(grafo, 2, 3, 2);
                insere_aresta(grafo, 2, 6, 5);

                insere_aresta(grafo, 3, 6, 2);

                insere_aresta(grafo, 4, 5, 1);

                break;

            default:
                destroi_grafo(grafo);
        }

    }while(escolha != 99);

    return;
}
