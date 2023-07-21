#include "truco.h"
#include <stdio_ext.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <netinet/in.h> //inet_addr
#include <pthread.h>
#include <unistd.h>

typedef struct mensagem {

    char *msg;
    int lenght;
    int bytes_read;
} Mensagem;

char *IP();

void enviarInt (int idSocket, int valor);

int recebeInt (int idSocket);

void enviarStr(int idSocket, char *str);

Mensagem *recebeStr (int idSocket);