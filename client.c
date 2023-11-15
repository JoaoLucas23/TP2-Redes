#include "common.h"
#include "blog.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSZ 1024

int client_id = 0;

void usage(int argc, char **argv) {
    printf("usage %s <%s> <%s>\n", argv[0],argv[1],argv[2]);
    printf("example: %s 127.0.0.1 51511", argv[0]);
    exit(EXIT_FAILURE);
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

    addrtostr(addr, addrstr, BUFSZ);
    while (1)
    {   
        char buf[BUFSZ];
        memset(buf, 0, BUFSZ);
        fgets(buf, BUFSZ-1, stdin);

        le_mensagem_cliente(buf, operation);

        operation->client_id = client_id;

        size_t count = send(s, operation, sizeof(struct BlogOperation), 0);
        if(count != sizeof(struct BlogOperation)) {
            logexit("send");
        }

        count = recv(s, operation, sizeof(struct BlogOperation), 0);

        le_resposta_servidor(operation, &client_id);

    }
    close(s);
    exit(EXIT_SUCCESS);
}