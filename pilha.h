#ifndef PILHA_
#define PILHA_
#include "pilha.c"

    typedef struct {
        int local;
        int pilha*;
    }Pilha;
    typedef Pilha *p_pilha;

    p_pilha criarPilha();
    void empilhar(p_pilha p,int conteudo);
    int desempilhar(p_pilha p);
    void destroiPilha(p_pilha p);
    int pilhaVazia(p_pilha p);

#endif
