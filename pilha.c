
typedef struct {
    int local;
    int *pilha;
}Pilha;

typedef Pilha *p_pilha;

p_pilha criarPilha(){
    p_pilha p = malloc(sizeof(Pilha));
    p->local = -1;
    p->pilha = malloc ( 50 * sizeof(int));
    return p;
}

void empilhar(p_pilha p,int conteudo){
    p->local++;
    p->pilha[p->local] = conteudo;
}

int desempilhar(p_pilha p){
    int conteudo;

    conteudo = p->pilha[p->local];

    p->local--;
    return conteudo;
}

void destroiPilha(p_pilha p){
    free(p->pilha);
    free(p);
}

int pilhaVazia(p_pilha p){
    if(p->local == -1)
        return 1;
    return 0;
}
