#include "common.h"
#include "blog.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#define BUFSZ 1024
#define MAX_CLI 10

struct ClienteConectado* clientes_conectados;
struct Topico* topicos_criados;

int* qtd_topicos;
int qtd_clientes;

int sockets_ativos[MAX_CLI];

void usage(int argc, char **argv) {
    printf("usage %s <v4|v6> <server port>\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

struct client_data {
    int csock;
    struct sockaddr_storage storage;
};

void * client_thread(void *data) {     
    struct client_data *cdata = (struct client_data *)data;
    struct sockaddr *caddr = (struct sockaddr *) (&cdata->storage);
    
    char caddrstr[BUFSZ];
    addrtostr(caddr, caddrstr, BUFSZ);

    struct BlogOperation* operation = malloc(sizeof(struct BlogOperation));
    iniciaBlogOperation(operation);
    struct ClienteConectado* cliente = malloc(sizeof(struct ClienteConectado));
    iniciaCliente(cliente, clientes_conectados, operation);
    sockets_ativos[qtd_clientes] = cdata->csock;
    qtd_clientes++;
    printf("client %d connected\n", cliente->id);
    size_t count = send(cdata->csock, operation, sizeof(struct BlogOperation), 0);
    if(count != sizeof(struct BlogOperation)) {
        logexit("send");
    }

    while (1)
    {
        //gera_resposta(operation, topicos_criados);
        count = recv(cdata->csock, operation, sizeof(struct BlogOperation), 0);

        trata_mensagem_cliente(operation, cliente,topicos_criados,qtd_topicos);
        imprime_mensagem_servidor(operation);

        if(operation->operation_type==2) {
            printf("ENTROU NO IF - type: %d server: %d content: %s\n", operation->operation_type,operation->server_response, operation->content);
            /*count = send(cdata->csock, operation, sizeof(struct BlogOperation),0);
            if(count != sizeof(struct BlogOperation)) {
                logexit("send");
            }*/
            for (int i = 0; i < qtd_clientes; i++)
            {
                send(sockets_ativos[i], operation, sizeof(struct BlogOperation),0);
            }
        }
        else {
            count = send(cdata->csock, operation, sizeof(struct BlogOperation), 0);
            if(count != sizeof(struct BlogOperation)) {
                logexit("send");
            }
        }

    }
    close(cdata->csock);
    free(cdata);
    pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    if(argc < 3) {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if(server_sockaddr_init(argv[1], argv[2], &storage) != 0) {
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1) {
        logexit("socket");
    } 

    int enable=1;
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) != 0) {
        logexit("setsockopt");
    }

    struct sockaddr *addr = (struct sockaddr *) (&storage);

    if(bind(s, addr, sizeof(storage)) != 0){
        logexit("bind");
    }
    if(listen(s, 10) != 0){ //quantidade de conexoes pendentes para tratamento
        logexit("listen");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);
    printf("bound to %s, waiting connections \n", addrstr);

    clientes_conectados = (struct ClienteConectado *)malloc(10 * sizeof(struct ClienteConectado));
    topicos_criados = (struct Topico *)malloc(10 * sizeof(struct Topico));
    qtd_topicos = (int*)malloc(sizeof(int));

    qtd_clientes = 0;
    (*qtd_topicos) = 0;
    int client_check[MAX_CLI];
    memset(client_check, 0, MAX_CLI*sizeof(int));

    while (1)
    {
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *) (&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        int csock = accept(s, caddr, &caddrlen);
        if(csock == -1) {
            logexit("accept");
        }

        struct client_data *cdata = malloc(sizeof(*cdata));
        cdata->csock = csock;
        memcpy(&(cdata->storage), &storage, sizeof(storage));

        int i=0;
        while (i<MAX_CLI) i++;
                   
        send(csock,"TESTE",BUFSZ,0);
        pthread_t tid;
        pthread_create(&tid, NULL, client_thread, cdata);
    }

    exit(EXIT_SUCCESS);
};
