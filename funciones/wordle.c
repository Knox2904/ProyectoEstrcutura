#include "TDAs/TDAs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "wordle.h"

Wordle *crearWrodle(int maxIntentos , int largoPalabra , const char *palabraObjetivo ){
    
    Wordle *partida = malloc(sizeof(Wordle)) ; 
    partida->maxIntentos = maxIntentos ; 
    partida->largoPalabra = largoPalabra ; 
    partida->palabraObjetivo = strdup(palabraObjetivo) ; 
    partida->intentoActual = 0 ; 

    //inicio de la matriz 
    partida->TablaWordle = malloc(sizeof(char*) * maxIntentos) ;
    for(int i = 0 ; i < maxIntentos ; i++){
        partida->TablaWordle[i] = calloc(largoPalabra + 1 , sizeof(char)) ; 
    }

    return partida; 
}


void guardarIntento(Wordle *partida , const char *intento) {
    if (partida->intentoActual >= partida->maxIntentos) return NULL;

    strncpy(partida->TablaWordle[partida->intentoActual], intento, partida->largoPalabra);
    partida->intentoActual++;
}

void destruirWordle(Wordle *partida) ; 