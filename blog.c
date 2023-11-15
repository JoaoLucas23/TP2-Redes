#include "blog.h"

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

void iniciaBlogOperation(struct BlogOperation* operation) {
    operation->client_id = 0;
    operation->operation_type = 1;
    operation->server_response = 0;
    strcpy(operation->topic, "");
    strcpy(operation->content, "");
};

void imprime_mensagem_servidor(struct BlogOperation* operation) {
    printf("entrei aqui %d\n", operation->operation_type);
    switch (operation->operation_type)
    {
    case 1:
        printf("client %d connected\n", operation->client_id);
        break;
    case 2:
        printf("new post added in %s by %d\n", operation->topic, operation->client_id);
        break;
    case 4:
        printf("client %d subscribed to %s\n", operation->client_id, operation->topic);
        break;
    case 5:
        printf("client %d disconnected\n", operation->client_id);
        break;
    case 6:
        printf("client %d unsubscribed to %s\n", operation->client_id, operation->topic);
        break;
    default:
    printf("ENTREI NO DEFAULT\n");
        break;
    }
}

void le_mensagem_cliente(char* buf, struct BlogOperation* operation) {
    buf = strtok(buf, " \n");
    operation->operation_type = 1;
    strcpy(operation->content,strtok(NULL, ""));
}