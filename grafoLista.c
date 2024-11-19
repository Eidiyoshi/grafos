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

void dijsktra(p_grafo g, int origem, int alvo, int apenasAlvo ){
    int *distancia = malloc(g->n * sizeof(int));
    int *visitado = malloc(g->n * sizeof(int));
    int *pai = malloc(g->n * sizeof(int));
    int v,achado = 0;
    p_no listaAdjacente;
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

        if(v == alvo) achado = 1;

        for(listaAdjacente = g->adjacencia[v]; listaAdjacente != NULL; listaAdjacente = listaAdjacente->prox){ // considerando todos os possiveis lugares pra ir
            int l = distancia[v] + listaAdjacente->d;
            int oo = distancia[listaAdjacente->v];
            if(!visitado[listaAdjacente->v] && (( distancia[v] + listaAdjacente->d ) <= distancia[listaAdjacente->v])){
                distancia[listaAdjacente->v] = distancia[v] + listaAdjacente->d;
                pai[listaAdjacente->v] = v;
                enfileiraPrioridade(fPrioridade, g->n, listaAdjacente->v, distancia[listaAdjacente->v]);
                visitado[v] = 1;
            }
        }
    }

    if(apenasAlvo){
        if(achado){
            int s;
            printf("Alvo achado\n");
            printf("Distancia de %d\n",distancia[alvo]);
            printf("Caminho Inverso: ");
            s = alvo;
            while(s != origem){
                s = pai[s];
                printf("%d, ",s);
            }
            printf("\n\n");
        }else{
            printf("Alvo nao achado\n\n");
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
    printf("Grafos de Lista\n");

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
