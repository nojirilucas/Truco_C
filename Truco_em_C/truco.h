#include "strUtils.h"

#define JOGADORES 4
#define DUPLA 2

typedef struct{
	u_int32_t valor;
	char nome[3];
} Carta;

typedef struct{
	u_int32_t numero;
	int socket;
	Carta mao[3];
} Jogador;

typedef struct{
	u_int32_t numeroJogador[4];
	u_int32_t tamMesa;
	Carta cartas[4];
} Mesa;

// Configura o valor e o nome das cartas na estrutura baralho.
void construirBaralho (Carta *baralho);

// Recebe um baralho configurado e altera a posição das cartas
// de forma aleatória.
void embaralhar (Carta *baralho);

// Seleciona as primeiras cartas do baralho e distribui entre os
// jogadores.
void distribuirCartas (Jogador *jogadores, Carta *baralho);

// Recebe a mesa do jogo (com cartas dos participantes) e verifica
// se alguma das equipes ganhou o turno e retorna a informação atra-
// -vés de um valor inteiro.
int vencerTurno (Mesa *mesa);

// Verifica se alguma das duplas venceu a rodada (3 turnos) ou se 
// houve empate e retorna a informação através de um inteiro.
int vencerRodada (int *placarRodada, int primeiroTurno, int turno);
