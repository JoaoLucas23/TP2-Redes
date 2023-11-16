#include "topico.h"

int verifica_topico(char* topico, struct Topico* topicos_criados, int qtd_topicos){
    for (int i = 0; i < qtd_topicos; i++)
    {
        if(strcmp(topico,topicos_criados[i].nome)==0) {
            return i;
        } 
    }
    return 0;
}

void cria_topico(char* nome_topico, struct Topico* topicos_criados, int* qtd_topicos){
    struct Topico topico;
    topico.id = *qtd_topicos;
    topico.nome = strdup(nome_topico);

    topicos_criados[*qtd_topicos] = topico;

    (*qtd_topicos)++;
}

char* lista_topicos_criados(struct Topico* topicos_criados, int qtd_topicos){

    if (qtd_topicos <= 0)
    {
        return "";
    }
    
    printf("passou dos topicos\n");

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
        if(topicos_criados[i].nome == topico) return i;
    }
    return -1;
}