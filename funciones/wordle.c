
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "wordle.h"

Wordle *crearWordle(int maxIntentos , int largoPalabra , const char *palabraObjetivo ){
    
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
    if (partida->intentoActual >= partida->maxIntentos) return ;

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

int conseguirNumeroAleatorio(int max) {
    return rand() % max;
}

void reiniciarParida(Wordle *partida , const char *NuevaPalabra){

    partida->intentoActual = 0 ; 

    free(partida->palabraObjetivo) ; 
    partida->palabraObjetivo = strdup(NuevaPalabra) ; 

    for(int i =0 ; i < partida->maxIntentos ; i++){
        memset(partida->TablaWordle[i], 0, partida->largoPalabra + 1);
    }
}

void evaluarIntento(const Wordle *partida , const char *intento , char * resultado)
{
    int largo = partida ->largoPalabra;
    const char *objetivo = partida ->palabraObjetivo;
    bool usadas[32] = {0}; 

    for (int i = 0; i < largo ; i++)
    {
        if (intento[i] == objetivo[i])
        {
            resultado[i] = 'G';
            usadas[i] = true;
        }

        else
        {
            resultado[i] = 'U'; //unkown, desconocido, se decide despues

        }
    }

    for (int i = 0; i < largo; ++i) 
    {
        if (resultado[i] == 'U') {
            resultado[i] = 'I'; // Incorrecta por que da la gana
            for (int j = 0; j < largo; ++j) {
                if (!usadas[j] && intento[i] == objetivo[j]) {
                    resultado[i] = 'Y';
                    usadas[j] = true; // se marca la letra para que no se cuente de nuevo si es que pasa
                    break;
                }
            }
        }
    }

    resultado[largo] = '\0'; // se aseguta el fin del string o palabra.
}

void imprimirResultado(const char* intento, const char* resultado, int largo) {
    for (int i = 0; i < largo; ++i) {
        char letra = intento[i];
        char eval = resultado[i];
        if (eval == 'G') { // Verde - correcta en posición
            printf("\033[1;32m%c\033[0m", letra);
        } else if (eval == 'Y') { // Amarillo - está pero en otra posición
            printf("\033[1;33m%c\033[0m", letra);
        } else if (eval == 'I') { // Rojo - incorrecta
            printf("\033[1;31m%c\033[0m", letra); // Rojo brillante
        } else { // Por si acaso hay otra marca (ej: 'U')
            printf("%c", letra);
        }
    }
    printf("\n");
}

void mostrarWordleColoreado(Wordle *partida) {
    for (int i = 0; i < partida->intentoActual; i++) {
        char resultado[32];
        evaluarIntento(partida, partida->TablaWordle[i], resultado);
        imprimirResultado(partida->TablaWordle[i], resultado, partida->largoPalabra);
    }
}

void reglasWordle() {

    printf("Objetivo : Adivinar una palabra de dificultad variable en una cantidad de intentos variables \n") ;
    printf("Facil -> largo de 3 a 6 letras , con 6 intentos\n") ;
    printf("Normal -> largo de 4 a 6 letras , con 5 intentos \n") ;
    printf("Dificil -> largo con 7 a 35 letras con 3 intentos (suerte 🙏💀 )\n") ;
    puts("") ; 
    puts("Como se juega :") ; 
    puts("1) escribe una palabra de largo cambiante , segun la dificultad elegida , en cada intento") ; 
    puts("2) despues de cada intento las letras cambiaran de color para darte pistas siguento el siguiente codigo : ") ;
    puts("verde -> la letra esta en la posicion correcta y la letra es correcta ") ;
    puts("amarillo -> la letra esta en la palabra pero en la posicion incorrecta ") ;
}