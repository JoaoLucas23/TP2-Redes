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

struct ClienteConectado* clientes_conectados;

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
    printf("client %d connected\n", cliente->id);
    
    while (1)
    {
        size_t count = recv(cdata->csock, operation, sizeof(struct BlogOperation), 0);
        trata_mensagem_cliente(operation, cliente);
        imprime_mensagem_servidor(operation);

        gera_resposta(operation);
        count = send(cdata->csock, operation, sizeof(struct BlogOperation), 0);
        if(count != sizeof(struct BlogOperation)) {
            logexit("send");
        }
        
    }
    close(cdata->csock);
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

        pthread_t tid;
        pthread_create(&tid, NULL, client_thread, cdata);
    }

    exit(EXIT_SUCCESS);
};
