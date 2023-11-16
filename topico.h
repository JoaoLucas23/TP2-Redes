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

int verifica_topico(int topico, int* topicos_criados, int qtd_topicos);
struct Topico cria_topico(char* topico, int* topicos_criados, int qtd_topicos);
void lista_topicos_criados();
int traduz_topico(char* topico, struct Topico* topicos_criados, int qtd_topicos);
