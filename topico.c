#include "topico.h"

int verifica_topico(int topico, struct Topico* topicos_criados, int qtd_topicos){
    for (int i = 0; i < qtd_topicos; i++)
    {
        if(topico==topicos_criados[i].id) return 0;
    }
    return 1;
}

struct Topico cria_topico(char* nome_topico, int* topicos_criados, int qtd_topicos){
    struct Topico topico;
    topico.id = qtd_topicos;
    topico.nome = nome_topico;
    return topico;
}

void lista_topicos_criados(struct Topico* topicos_criados, int qtd_topicos){
    for (int i = 0; i < qtd_topicos; i++)
    {
        printf("%s\n",topicos_criados[i].nome);
    }
}

int traduz_topico(char* topico, struct Topico* topicos_criados, int qtd_topicos) {
    for (int i = 0; i < qtd_topicos; i++)
    {
        if(topicos_criados[i].nome == topico) return i;
    }
    return -1;
}