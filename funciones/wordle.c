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

void destruirWordle(Wordle *partida){
    for(int i = 0 ; i < partida->maxIntentos ; i++){
        free(partida->TablaWordle[i]) ; 
    }
    free(partida->TablaWordle) ; 
    free(partida->palabraObjetivo) ; 
    free(partida) ; 
}

void reiniciarParida(Wordle *partida , const char *NuevaPalabra){

    partida->intentoActual = 0 ; 

    free(partida->palabraObjetivo) ; 
    partida->palabraObjetivo = strdup(NuevaPalabra) ; 

    for(int i =0 ; i < partida->maxIntentos ; i++){
        memset(partida->TablaWordle[i], 0, partida->largoPalabra + 1);
    }
}