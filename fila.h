#ifndef FILA_
#define FILA_
#include "fila.c"

    typedef struct {
        int local;
        int fila*;
    }Fila;
    typedef Fila *p_fila;

    p_fila criarFila();
    void enfileira(p_fila p,int conteudo);
    int desenfileira(p_fila p);
    void destroiFilha(p_fila p);
    int filaVazia(p_fila p)

#endif
