#include "usuario.h"

char *IP(){
    
    char *ip = "127.0.0.1";
    while (1) {
        printf("Digite o endereço: ");
        scanf("%15[^\n]s", ip);
        __fpurge(stdin);
        if (!verificaIp(ip)) {
            return ip;            
        }
    }
}

void enviarInt (int idSocket, int valor){

    int v = htonl (valor);
    int bytes_write = write (idSocket, &v, sizeof(int));
    if (bytes_write == 0){
        printf ("Erro\n");
    }else if (bytes_write < 0){
        printf ("Erro\n");
    }
}

int recebeInt (int idSocket){

    int v;
    int bytes_read = recv(idSocket, &v, sizeof(int), 0);
    if (bytes_read < 0){
        printf ("Erro\n");
        return -1;
    }
    return ntohl (v);
}

void enviarStr (int idSocket, char *str){

    int lenght = strlen(str) + 1;
    enviarInt (idSocket, lenght);
    
    int bytes_write = write(idSocket, str, lenght);
    if (bytes_write == 0){
        printf ("Erro\n");
    }else if (bytes_write < 0){
        printf ("Erro\n");
    }
}

Mensagem *recebeStr (int idSocket){
        
    Mensagem *msg = malloc (sizeof(Mensagem));
    msg->lenght = recebeInt (idSocket);
    if(msg->bytes_read < 0){
        printf ("Erro: erro ao ler string.\n");
        msg->msg = NULL;
        msg->lenght = 0;
        return msg;
    }
    msg->msg = malloc(msg->lenght);
    msg->bytes_read = recv(idSocket, msg->msg, msg->lenght, 0);
    return msg;
}