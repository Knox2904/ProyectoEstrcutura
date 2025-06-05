#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "TDAs/TDAs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
    
    char** TablaWordle ; // usa la fila como intento para poner ahi la palabra 
    char*  palabraObjetivo ; // se explica solo , es la palabra que el jugador adivina 
    int maxIntentos ;   // la cantidad de intentos maximos 
    int largoPalabra ;  // el largo de la palabra , se usa para controlar que lo que el jugador quiero adivinar sea del mismo largo que la palabra a adivinar 
    int intentoActual ;  // usalo para saber la fila en donde escribir 

}Wordle;

//funcion para poder crear una nueva partida de wrodle , iniciando la matriz y guardando el largo de la palabra y el maximo de intentos para el jugador 
Wordle *crearWrodle(int maxIntentos , int largoPalabra , const char *palabraObjetivo ) ;

//funcion para poder guardar un intento en el wordle , retorna NULL si se exedio el maximo de intentos o ya estas en el maximo
void guardarIntento(Wordle *partida , const char *intento) ; 

//esta funcion DESTRUYE NO REINICIA  el wordle
void destruirWordle(Wordle *partida) ; 

//esta funcion REINICIA LA PARTIDA , y necesita una nueva palabra
void reiniciarParida(Wordle *partida , const char *NuevaPalabra) ; 

#endif