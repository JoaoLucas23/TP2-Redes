#include "topico.h"

int verifica_topico(char* topico, struct Topico* topicos_criados, int qtd_topicos){
    
    for (int i = 0; i < qtd_topicos; i++)
    {
        if(strcmp(topico,topicos_criados[i].nome)==0) {
            return i+1;
        } 
    }
    return 0;
}

int cria_topico(char* nome_topico, struct Topico* topicos_criados, int* qtd_topicos){
   
    struct Topico topico;
    topico.id = *(qtd_topicos) + 1;
    topico.nome = strdup(nome_topico);

    topicos_criados[*qtd_topicos] = topico;
    (*qtd_topicos)++;

    return topico.id;
}

char* lista_topicos_criados(struct Topico* topicos_criados, int qtd_topicos){

    if (qtd_topicos <= 0)
    {
        return "";
    }

    // Calculando o tamanho total necessário para a string
    int tamanhoTotal = 0;
    for (int i = 0; i < qtd_topicos; i++) {
        tamanhoTotal += strlen(topicos_criados[i].nome) + 1; // +2 para a vírgula e o espaço
    }

    // Alocando memória para a string resultante
    char* resultado = (char*)malloc(tamanhoTotal + 1); // +1 para o caractere nulo de terminação
    if (resultado == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    // Construindo a string
    resultado[0] = '\0'; // Inicializando a string vazia
    for (int i = 0; i < qtd_topicos; i++) {
        strcat(resultado, topicos_criados[i].nome);
        if (i < qtd_topicos - 1) {
            strcat(resultado, ";\n");
        }  
    }

    return resultado;
}

void imprime_topicos_criados(char* topicos){

    printf("%s", topicos);
    
}

int traduz_topico(char* topico, struct Topico* topicos_criados, int qtd_topicos) {
    for (int i = 0; i < qtd_topicos; i++)
    {
        if(strcmp(topicos_criados[i].nome,topico)==0) return topicos_criados[i].id;
    }
    return -1;
}