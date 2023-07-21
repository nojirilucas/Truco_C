
#include "usuario.h"

int Conexoes;

pthread_mutex_t ConexoesMutex;

pthread_mutex_t iniciarPartida;

Jogador jogadores[4];

Carta baralho[40];

Mesa *mesaJogo;

int menuComando(char *buffer);

void escutaSolicitacao();

void *escutaCliente(void *idSocket);

void desconecta();

void enviarCartas();

void truco(int jogadorSolicitante);

void enviarMesa ();

void placarRodada (int valorRodada);

void placar (char *msg);

int jogadorAnterior (int vezJogador);

int proximoJogador (int vezJogador);

void controleJogo();

void numero();
