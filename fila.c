#include <limits.h>

typedef struct {
    int tamanho;
    int *fila;
    int *filaDistancia;
}Fila;

typedef Fila *p_fila;

p_fila criarFila(){
    p_fila p = malloc(sizeof(Fila));
    p->tamanho = -1;
    p->fila = malloc( 50 * sizeof(int));
    p->filaDistancia = malloc( 50 * sizeof(int));
    for(int i = 0; i < 50; i ++ ){
        p->fila[i] = -1;
        p->filaDistancia[i] = INT_MAX;
    }
    return p;
}

void enfileira(p_fila p,int conteudo, int distancia){
    p->tamanho++;
    p->fila[p->tamanho] = conteudo;
    p->filaDistancia[p->tamanho] = distancia;

}

int desenfileira(p_fila p){
    int conteudo;

    conteudo = p->fila[0];

    for( int i = 0; i < p->tamanho; i++){
        p->fila[i] = p->fila[i+1];
        p->filaDistancia[i] = p->filaDistancia[i+1];
    }
    p->tamanho--;

    return conteudo;
}

void destroiFila(p_fila p){
    free(p->fila);
    free(p);
}

int filaVazia(p_fila p){
    if(p->tamanho == -1)
        return 1;
    return 0;
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
