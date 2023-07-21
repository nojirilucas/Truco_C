#include "cliente.h"

void error(const char *msg){
    perror(msg);
    exit(0);
}

void iniciaConexao (){

    struct sockaddr_in servidor;
        
    jogadorCliente.socket = socket(AF_INET , SOCK_STREAM , 0);
    if (jogadorCliente.socket == -1){
        
        printf("SOCKET N FOI CRIADO...\n");
        return;
    }   
    
    Mensagem *msg = malloc (sizeof(Mensagem));
    
    msg->msg = malloc(sizeof(char)*16);
    while(1){
        
        printf("Digite o endereço ip: ");
        scanf("%10[^\n]s", msg->msg);
        __fpurge(stdin);
        if(!verificaIp(msg->msg)){
            
            break;
        }
    }
    servidor.sin_addr.s_addr = inet_addr(msg->msg);
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(40001);
    memset(servidor.sin_zero, 0, sizeof servidor.sin_zero);
    
    if (connect(jogadorCliente.socket, (struct sockaddr *)&servidor , sizeof(servidor)) < 0){     
        error("Erro. conexão não estabelecida...");
        return;
    }
    int count = 0;
    char ok;
    while(count < 3){                
        recv(jogadorCliente.socket, &ok, sizeof (char), 0);
        count++;
    }
        printf("Por favor aguarde o inicio da partida.\n");
}

void comandos (int valorRodada, int com){
    
    printf("00 - Jogar Carta\n");

    if(!com){
        if(valorRodada == 2 ){
            printf("01 - Pedir truco\n");
        }else if(valorRodada == 10){
            printf("01 - Pedir Jogo\n");
        }
    }
}

void jogadas (int com){
    
    char buffer[3];    
    while(1){
        scanf("%2[^\n]s", buffer);
        __fpurge(stdin);
        if(!strncmp(buffer,"00" , 3)){
            enviarStr(jogadorCliente.socket, "00");
            jogar();
            return;
        }
        else if(!strncmp(buffer, "01", 3) && !com){
            enviarStr(jogadorCliente.socket, "01");
            return;
        }else{
            printf("Opção inválida, por favor digite novamente.\n");            
        }
    }
}

void decodificador (){
    
    Mensagem *msg;
    int valorRodada, valorMao10 = 0, com = 0;
    msg = recebeStr(jogadorCliente.socket);
    if(msg->bytes_read < 0){
        printf("Erro: falha ao receber a mensagem.\n");
        return;
    }
    printf (msg->msg);
    free(msg);
    while(1){
        sleep(1);
        msg = recebeStr (jogadorCliente.socket); 
        if(!strncmp(msg->msg, "10",3)){
            printf ("Sua vez de jogar jogador.\n");
            menuJogadas (com);
        }
        else if (!strncmp(msg->msg, "11", 3)){
            visualizarMesa ();
        }
        else if (!strncmp(msg->msg, "12", 3)){
            receberCartas ();
            verCarta ();        
        }
        else if (!strncmp(msg->msg, "13", 3)){
            valorRodada = recebeInt (jogadorCliente.socket);
            printf ("Valor da rodada: %d.\n", valorRodada);
        }
        else if (!strncmp(msg->msg, "14", 3)){            
            msg = recebeStr (jogadorCliente.socket);
            printf (msg->msg);
        } 
        else if (!strncmp(msg->msg, "15", 3)){
            printf ("Partida finalizada.\n");
            exit(1);
        }
        free(msg);
    }
    close(jogadorCliente.socket);
}

void receberCartas (){

    int numeroCarta;
    Mensagem *msg;
    for (numeroCarta = 0; numeroCarta < 3; numeroCarta++){
        
        msg = recebeStr (jogadorCliente.socket);
        if (msg->bytes_read < 0){
            printf ("Erro: falha ao receber as cartas.\n");
            return;
        }
        memcpy (jogadorCliente.mao[numeroCarta].nome, msg->msg, msg->lenght);
        jogadorCliente.mao[numeroCarta].valor = recebeInt (jogadorCliente.socket);
    }
}

void jogar (){

    while (1){
        
        verCarta ();
        printf("Escolha a carta que voce vai jogar: ");
        char resposta[3];
        scanf("%2[^\n]s", resposta);
        __fpurge(stdin);
        int k;
        while(k < 3){
            if(jogadorCliente.mao[k].nome[0] == resposta[0] && jogadorCliente.mao[k].nome[1] == resposta[1]){            
                enviarStr(jogadorCliente.socket, resposta);
                jogadorCliente.mao[k].nome[0] = 0;
                jogadorCliente.mao[k].nome[1] = 0;
                enviarInt(jogadorCliente.socket, jogadorCliente.mao[k].valor);
                return;
            }
            k++;
        }
        printf("Voce nao tem esta carta\n");
    }
}

void verCarta (){
    
    int i;
    printf("Suas cartas: \n");
    for(i = 0; i < 3; i++){
        if(jogadorCliente.mao[i].nome[0] != 0){
            printf("%s\t",jogadorCliente.mao[i].nome);
        }
    }
    printf("\n");
}

void verMesa (){
    
    int i;
    int tamMesa;
    Mensagem *msg;
    printf("Cartas na Mesa: \n");
    tamMesa = recebeInt (jogadorCliente.socket);
    while(i < tamMesa){
        msg = recebeStr(jogadorCliente.socket);
        printf ("%s\t", msg->msg);
        free (msg);
        i++;
    }
    printf("\n");
}

int main(){       

    iniciaConexao();
    
    if(jogadorCliente.socket == -1){
        
        printf("ERRO.\n");
        return 1;
    }
    decodificador ();
    
    return 0;
}
