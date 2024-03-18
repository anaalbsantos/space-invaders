#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "driver.h"


int digit_display(int num){
    switch(num){
        case 0: return 0x40;
        case 1: return 0x79;
        case 2: return 0x24;
        case 3: return 0x30;
        case 4: return 0x19;
        case 5: return 0x12;
        case 6: return 0x02;
        case 7: return 0x78;
        case 8: return 0x00;
        case 9: return 0x18;
        default: return 0xFF;
    }
}

unsigned int decimal_to_display(int numero, int casas){
    casas = casas-1;
    unsigned int hex_resultado = 0, pow_hex=casas, casasAdicionais;
    int digito;
    int potencia_10 = pow(10,casas);
    int primeiroSignificativo = 0;
    while(potencia_10>0){
       /*if(primeiroSignificativo==0){ //Para o caso de números menor que o núemro de casas, os displays à esquerda ficarão apagados. Em vez de mostrar no display "0003" vai ficar "   3"
            if(numero<potencia_10) digito = -1;
            else {
                primeiroSignificativo = 1;
                digito = numero/potencia_10;
            }
        }
        else */ 
        digito = numero/potencia_10;

        casasAdicionais = pow(16, pow_hex); //Pular os primerios digitos
        hex_resultado += pow(casasAdicionais,2) * digit_display(digito);
        pow_hex--;

        numero=numero%potencia_10;
        potencia_10=potencia_10/10;
    }
    return hex_resultado;
}

int *binary_transform(unsigned int numero, int tam){
	tam--;
	int *vetor;
	vetor = malloc(sizeof(int)*(tam+1));
	for(int i = 0; i < tam+1; i++){
		if(numero%2 == 1) vetor[tam-i] = 1;
		else vetor[tam-i] = 0;
		numero /= 2;
	}
	return vetor;
}
