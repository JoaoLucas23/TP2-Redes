#include "common.h"
#include "blog.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define BUFSZ 1024

int* client_id = 0;

void usage(int argc, char **argv) {
    printf("usage %s <%s> <%s>\n", argv[0],argv[1],argv[2]);
    printf("example: %s 127.0.0.1 51511", argv[0]);
    exit(EXIT_FAILURE);
}

void* input_thread(void *sock) {
    struct BlogOperation* operation = malloc(sizeof(struct BlogOperation));
    char buf[BUFSZ];
    while (1)
    {   
        memset(buf, 0, BUFSZ);
        fgets(buf, BUFSZ-1, stdin);

        le_mensagem_cliente(buf, operation, *(client_id));

        size_t count = send(sock, operation, sizeof(struct BlogOperation), 0);
        if(count != sizeof(struct BlogOperation)) {
            logexit("send");
        }
    }
    free(operation);
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    if(argc < 3) {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if(addrparse(argv[1], argv[2], &storage) != 0) {
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1) {
        logexit("socket");
    } 

    struct sockaddr *addr = (struct sockaddr *) (&storage);

    if (connect(s, addr, sizeof(storage)) != 0) {
        logexit("connect");
    }

    char addrstr[BUFSZ];

    struct BlogOperation* operation = malloc(sizeof(struct BlogOperation));
    client_id = malloc(sizeof(int));

    addrtostr(addr, addrstr, BUFSZ);
    pthread_t tid;
    long sockNum = (long)s;
    pthread_create(&tid, NULL, input_thread, (void *)sockNum);

    while (1)
    {   
        recv(s, operation, sizeof(struct BlogOperation), 0);
        le_resposta_servidor(operation, client_id);
    }
    close(s);
    free(operation);
    free(client_id);
    exit(EXIT_SUCCESS);
}