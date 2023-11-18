#include "blog.h"
#include "topico.h"

#define BUFSZ 1024

void iniciaCliente(struct ClienteConectado* cliente, struct ClienteConectado* clientes_conectados, struct BlogOperation* operation) {
    for(int i=0; i<10; i++) {
        if (clientes_conectados[i].id == 0)
        {
            clientes_conectados[i].id = i+1;
            cliente->id = i+1;
            cliente->qtd_topicos_inscritos = 0;
            cliente->topicos_inscritos = (int*)malloc(10*sizeof(int));
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
        break;
    }
}

int traduz_acao(char* acao) {
    char* acoes_possiveis[6] = {
        "connect",
        "publish",
        "list",
        "subscribe",
        "exit",
        "unsubscribe",
    };
    for (int i = 0; i < 6; i++)
    {
        if(strcmp(acao, acoes_possiveis[i])==0){
            return i+1;
        }
    }
    return -1;
}

void le_mensagem_cliente(char* buf, struct BlogOperation* operation, int client_id) {
    buf = strtok(buf, " \n");
    operation->operation_type = traduz_acao(buf);
    operation->server_response = 0;
    operation->client_id = client_id;

    switch (operation->operation_type)
    {
    case 1:
        printf("conectando\n");
        break;
    case 2:
        buf = strtok(NULL, " \n");
        strcpy(operation->topic,buf);
        memset(buf, 0, BUFSZ);
        fgets(buf, BUFSZ-1, stdin);
        strcpy(operation->content,buf);
        break;
    case 3:    
        printf("topicos criados:\n");
        break;
    case 4:
        buf = strtok(NULL, " \n");
        strcpy(operation->topic,buf);
        break;
    case 5:
        printf("desconectando\n");
        break;
    case 6:
        buf = strtok(NULL, " \n");
        strcpy(operation->topic,buf);
        break;
    default:
        break;
    }
    
}

void le_resposta_servidor(struct BlogOperation* operation, int* client_id) {
    if (operation->server_response==0)
    {
        return;
    }

    switch (operation->operation_type)
    {
    case 1:
        *(client_id) = operation->client_id;
        break;
    case 2:
        printf("new post added in %s by %d\n%s\n",operation->topic,operation->client_id,operation->content);
    break;
    case 3:
        imprime_topicos_criados(operation->content);
    default:
        break;
    } 
    
}

void inscreve_cliente_topico(char* topico, struct ClienteConectado* cliente, int tipo_operacao,struct Topico* topicos, int* qtd_topicos) {

    int topico_id = verifica_topico(topico,topicos,*qtd_topicos);

    if (topico_id==0) {
        topico_id = cria_topico(topico, topicos, qtd_topicos);
        printf("topicos_inscritos[%d] = %d\n",cliente->qtd_topicos_inscritos,topico_id);
        cliente->topicos_inscritos[cliente->qtd_topicos_inscritos] = topico_id;
        cliente->qtd_topicos_inscritos++;
    }
    else if (tipo_operacao==4) printf("error: already subscribed\n");
}

void desinscreve_cliente_topico(int topico, struct ClienteConectado* cliente) {
    printf("cliente desinscrito\n");

}

void trata_mensagem_cliente(struct BlogOperation* operation, struct ClienteConectado* cliente, struct Topico* topicos, int* qtd_topicos) {

    operation->server_response = 1;

    int topico = traduz_topico(operation->topic,topicos, *qtd_topicos);

    if (operation->client_id == 0)
    {
        operation->client_id = cliente->id;
    }

    switch (operation->operation_type)
    {
    case 2:
        inscreve_cliente_topico(operation->topic,cliente,operation->operation_type,topicos, qtd_topicos);
        cria_novo_post(operation,topicos);
        break;
    case 3:
        memcpy(operation->content,lista_topicos_criados(topicos, *qtd_topicos),255);
        break;
    case 4:
        inscreve_cliente_topico(operation->topic,cliente,operation->operation_type,topicos, qtd_topicos);
        break;
    case 5:
        printf("client %d disconnected\n", cliente->id);
        break;
    case 6:
        desinscreve_cliente_topico(topico,cliente);
        break;
    default:
        break;
    }
}

