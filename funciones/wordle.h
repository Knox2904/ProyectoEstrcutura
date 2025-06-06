#ifndef WORDLE_H
#define WORDLE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct{
    
    char** TablaWordle ; // usa la fila como intento para poner ahi la palabra 
    char*  palabraObjetivo ; // se explica solo , es la palabra que el jugador adivina 
    int maxIntentos ;   // la cantidad de intentos maximos 
    int largoPalabra ;  // el largo de la palabra , se usa para controlar que lo que el jugador quiero adivinar sea del mismo largo que la palabra a adivinar 
    int intentoActual ;  // usalo para saber la fila en donde escribir 

}Wordle;

//funcion para poder crear una nueva partida de wrodle , iniciando la matriz y guardando el largo de la palabra y el maximo de intentos para el jugador 
Wordle *crearWordle(int maxIntentos , int largoPalabra , const char *palabraObjetivo ) ;

//funcion para poder guardar un intento en el wordle 
void guardarIntento(Wordle *partida , const char *intento) ; 

//esta funcion DESTRUYE NO REINICIA  el wordle
void destruirWordle(Wordle *partida) ; 

//esta funcion REINICIA LA PARTIDA , y necesita una nueva palabra
void reiniciarParida(Wordle *partida , const char *NuevaPalabra) ; 

//esta funcion retorna un numero aleatorio entre 1 y 40
int conseguirNumeroAleatorio(int max) ; 

//funcion que evalua las letras puestas por el jugador segun la palabra de su intento, si es que esta correcta la letra en el espacio:
// correcta = G , presente en la palabra = Y , incorrecta = I, G es color verde, Y es amarillo e I  es rojo.
void evaluarIntento(const Wordle *partida , const char *intento , char * resultado);

//Imprime
void imprimirResultado(const char* intento, const char* resultado, int largo);

//mustra la matriz del wordle .
void mostrarWordleColoreado(Wordle *partida) ; 
#endif