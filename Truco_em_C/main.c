#include "truco.h"

    int main(){

	Carta baralho[40];

	Cartas(baralho);
	for(i=0;i<4;i++){
		jogador[i].numero = i+1;
	}
	for(i=0;i<40;i++){
		printf("carta :%c%c\n",baralho[i].nome[0],baralho[i].nome[1]);
		printf("valor :%i\n",baralho[i].valor );
	}
	embaralhar(baralho);
	for(i=0;i<40;i++){
		printf("carta :%c%c\n",baralho[i].nome[0],baralho[i].nome[1]);
		printf("valor :%i\n",baralho[i].valor );
	}
	distribuircarta(baralho,jogador);
	for(i=0;i<4;i++){
		printf("\n%i\n",jogador[i].numero);
		printf("%c%c\n",jogador[i].mao[0],jogador[i].mao[1] );
		printf("%i\n",jogador[i].valorCarta[0]);
		printf("%c%c\n",jogador[i].mao[2],jogador[i].mao[3] );
		printf("%i\n",jogador[i].valorCarta[1]);
		printf("%c%c\n",jogador[i].mao[4],jogador[i].mao[5] );
		printf("%i\n",jogador[i].valorCarta[2]);
	}
    int l = 0;
    while(l!=0){
		Cartas(baralho);
		embaralhar(baralho);
		distribuircarta(baralho,jogador);
		l++;
    }
}