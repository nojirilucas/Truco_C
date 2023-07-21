#include "usuario.h"

Jogador jogadorCliente;

Mesa mesaCliente;

void iniciaConexao();

void decodificador (void);

void comandos(int valorRodada, int com);

void jogadas(int com);

void respostaAumentoAposta(int valorRodada);

void receberCartas();

void jogar();

void verMesa();

void verCarta();

#endif