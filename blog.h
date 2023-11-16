#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include "topico.h"

struct BlogOperation {
    int client_id;
    int operation_type;
    int server_response;
    char topic[50];
    char content[2048];
};

struct ClienteConectado{ 
    int id;
    int* topicos_inscritos;
    int qtd_topicos_inscritos;
};

void iniciaBlogOperation(struct BlogOperation* operation);

void iniciaCliente(struct ClienteConectado* cliente, struct ClienteConectado* clientes_conectados, struct BlogOperation* operation);

void imprime_mensagem_servidor(struct BlogOperation* operation);

void le_mensagem_cliente(char* buf, struct BlogOperation* operation, int client_id);

void le_resposta_servidor(struct BlogOperation* operation, int* client_id);

void trata_mensagem_cliente(struct BlogOperation* operation, struct ClienteConectado* cliente, struct Topico* topicos, int* qtd_topicos);

void cria_novo_post(struct BlogOperation* operation,struct Topico* topicos);