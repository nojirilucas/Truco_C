#include "servidor.h"

void escutaSolicitacao(){

    int socketLocal, socketCliente, sizeSockaddr, i;
    struct sockaddr_in servidor, cliente;
    
    pthread_mutex_init(&ConexoesMutex, NULL);
    pthread_mutex_init(&iniciarPartida, NULL);
    pthread_t threadsId[JOGADORES];
    
    socketLocal = socket(AF_INET, SOCK_STREAM, 0);

    if(socketLocal == -1){
        
        printf("Error...\n");
        return;
    }
        
    memset(servidor.sin_zero, 0, sizeof servidor.sin_zero);
    
    if(bind(socketLocal, (struct sockaddr *) &servidor, sizeof(servidor)) < 0){
        
        printf("Erro...\n");
        close(socketLocal);       
        exit(1);
    }
   
    listen(socketLocal, JOGADORES);
    
    sizeSockaddr = sizeof(struct sockaddr_in);
    Mensagem *args = malloc (sizeof(Mensagem));
    args->msg = senha;
    args->lenght = strlen (senha);

    while(Conexoes < JOGADORES - 1){

        socketCliente = accept(socketLocal, (struct sockaddr *) &cliente, 
            (socklen_t *) &sizeSockaddr);        
        if(socketCliente < 0){
            close(socketCliente);
        }else {
            printf ("Conexão com cliente: %d iniciada.\n", Conexoes);
        }
        args->bytes_read = socketCliente;
        if (pthread_create(&threadsId[Conexoes], NULL,
            autenticaUsuarios, (void *) args)){
            printf ("Erro: falha ao cadastrar usuário.\n");
        }
    }
    for (i = 0; i < JOGADORES; i++){
        pthread_join (threadsId[i], NULL);
    }
    close (socketLocal);
    pthread_mutex_destroy (&ConexoesMutex);
}

void desconecta(){

    int i;
    while(i < JOGADORES){
        close(jogadores[i].socket);
        i++;
    }
}

void controleJogo(){

    construirBaralho (baralho);
    int turnos, VEZ, jogadas, pedindoTruco;
    int valorRodada, resultadoRodada, resultadoTurno;
    int primeiroTurno, jogadorRodada = 0;
    int tentos[5] = {2, 4, 8, 10, 12};
    int placarTurno[2];
    int placarTotal[2] = {0,0};
    Mensagem *msg;
    
    for (VEZ = 0; VEZ < JOGADORES; VEZ++){
        enviarStr (jogadores[VEZ].socket, "Partida iniciada.\n");
    }

    while (1){
        sleep (1);
        JogadorNumero();
        enviarCartas ();
        sleep(1);
        valorRodada = 0;
        VEZ = jogadorRodada;
        if (JOGADORES <=4){
            
            placarTurno[0] = 0;
            placarTurno[1] = 0;
            placarRodada (tentos[valorRodada]);
            for(turnos = 0; turnos < 3; turnos++){
                mesaJogo = calloc (1, sizeof(Mesa));
                printf ("Turno: %d.\n", turnos);
                for (jogadas = 0; jogadas < JOGADORES; jogadas++){
                    printf ("Vez de: %d.\n", VEZ);
                    if (mesaJogo->tamMesa > 0){
                        enviarMesa();
                    }
                    enviarStr (jogadores[VEZ].socket, "10");
                    msg = recebeStr (jogadores[VEZ].socket);
                    if (!strncmp (msg->msg, "00", msg->lenght)){
                        msg = recebeStr (jogadores[VEZ].socket);
                        strncpy (mesaJogo->cartas[mesaJogo->tamMesa].nome, msg->msg, 3);
                        mesaJogo->cartas[mesaJogo->tamMesa].valor = recebeInt (jogadores[VEZ].socket);       
                        mesaJogo->numeroJogador[mesaJogo->tamMesa] = VEZ;
                        mesaJogo->tamMesa++;
                    }else{ 
                        enviarStr (jogadores[jogadoresAumento[1]].socket, "01");
                        pedindoTruco = 1;
                    }
                }
                    VEZ = proximoJogador (VEZ);
                    free (msg);
            } 
                
        }
        free (mesaJogo);  
    }
        char resultadoJogo[75];
        sprintf(resultadoJogo,"\n-------------------------------------------------------------------------------\n");
        enviarResultado(resultadoJogo);   
        sprintf(resultadoJogo ,"\n \t\t\t ____________________\n \t\t\t|                    |\n \t\t\t|  Pontuacao Geral   |\n \t\t\t|\033[34m  Dupla (0 - 2): %d \033[37m |\n"
                          " \t\t\t|\033[31m  Dupla (1 - 3): %d \033[37m |\n \t\t\t|____________________|\n ", placarTotal[0], placarTotal[1]);
        enviarResultado (resultadoJogo);
        jogadorRodada = proximoJogador(jogadorRodada);
        if(placarTotal[0] > 10 || placarTotal[1] > 10){
            break;
        }
    desconecta();
    }

int proximoJogador (int VEZ){

    if (VEZ == JOGADORES - 1){
        return 0;
    }
    VEZ++;
    return VEZ;
}

int jogadorAnterior (int VEZ){
    if (VEZ == 0){
        return (JOGADORES - 1);
    }
    VEZ--;
    return VEZ;
}

void enviarCartas() {

    printf ("Enviando cartas.\n");
    embaralhar (baralho);
    distribuirCartas (jogadores, baralho);
    int numeroJogador, numeroCarta;
    for (numeroJogador = 0; numeroJogador < JOGADORES; numeroJogador++){
        enviarStr (jogadores[numeroJogador].socket, "12");
        for (numeroCarta = 0; numeroCarta < 3; numeroCarta++){
            enviarStr (jogadores[numeroJogador].socket, jogadores[numeroJogador].mao[numeroCarta].nome);

            enviarInt (jogadores[numeroJogador].socket, 
                jogadores[numeroJogador].mao[numeroCarta].valor);
        }
    }
}

void enviarMesa (){
    
    int carta, jogador;
    for (jogador = 0; jogador < JOGADORES; jogador++){
        enviarStr (jogadores[jogador].socket, "11");
        enviarInt (jogadores[jogador].socket, mesaJogo->tamMesa);
        for (carta = 0; carta < mesaJogo->tamMesa; carta++){
            enviarStr (jogadores[jogador].socket, mesaJogo->cartas[carta].nome);
        }
    }
}

void truco (int pedindoTruco){

    int jogador;
    while(jogador < JOGADORES){
        if (jogador != pedindoTruco){
            enviarInt (jogadores[jogador].socket, pedindoTruco);
        }
    jogador++;
    }
}

void placarRodada (int valorRodada){

    int jogador;
    for (jogador = 0; jogador < JOGADORES; jogador++){
        enviarStr (jogadores[jogador].socket, "13");
        enviarInt (jogadores[jogador].socket, valorRodada);
    }
}

void placar (char *msg){

    int jogador;
    while(jogador < JOGADORES){
        enviarStr (jogadores[jogador].socket, "14");
        enviarStr (jogadores[jogador].socket, msg);
        jogador++;
    }
}

void numero(){
    char numeroJogador[75];
    int jogador;
    while(jogador < JOGADORES){
        enviarStr (jogadores[jogador].socket,"14");
        sprintf(numeroJogador,"Jogador %i \n",jogador);
        enviarStr(jogadores[jogador].socket,numeroJogador);
        jogador++;
    }
}

int main(){
    
    qtdeConexoes = 0;
    escutaSolicitacao ();
    
    controleJogo ();
    
    return 0;
}