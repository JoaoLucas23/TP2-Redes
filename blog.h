#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

struct BlogOperation {
    int client_id;
    int operation_type;
    int server_response;
    char topic[50];
    char content[2048];
};

void iniciaBlogOperation(struct BlogOperation* operation) {
    operation->client_id = 0;
    operation->operation_type = 1;
    operation->server_response = 0;
    strcpy(operation->topic, "");
    strcpy(operation->content, "");
};

struct ClienteConectado{ 
    int id;
    int* topicos_inscritos;
};

void iniciaCliente(struct ClienteConectado* cliente, struct ClienteConectado* clientes_conectados, struct BlogOperation* operation) {
    for(int i=0; i<10; i++) {
        if (clientes_conectados[i].id == 0)
        {
            clientes_conectados[i].id = i+1;
            cliente->id = i+1;
            operation->client_id = i+1;
            operation->server_response = 1;
            break;
        }
    }

}