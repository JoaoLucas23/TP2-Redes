#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

struct Topico
{
    int id;
    char* nome;
};

int verifica_topico(char* topico, struct Topico* topicos_criados, int qtd_topicos);
int cria_topico(char* nome_topico, struct Topico* topicos_criados, int* qtd_topicos);
char* lista_topicos_criados(struct Topico* topicos_criados, int qtd_topicos);
int traduz_topico(char* topico, struct Topico* topicos_criados, int qtd_topicos);
void imprime_topicos_criados(char* topicos);
