#ifndef WORDLE_H
#define WORDLE_H



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "TDAs/stack.h"
#include "TDAs/map.h"

typedef struct{
    
    char** TablaWordle ; // usa la fila como intento para poner ahi la palabra 
    char*  palabraObjetivo ; // se explica solo , es la palabra que el jugador adivina 
    int maxIntentos ;   // la cantidad de intentos maximos 
    int largoPalabra ;  // el largo de la palabra , se usa para controlar que lo que el jugador quiero adivinar sea del mismo largo que la palabra a adivinar 
    int intentoActual ;  // usalo para saber la fila en donde escribir 

}Wordle;

typedef struct {
    int numero_partida; // numero de la partida
    char* palabraObjetivo; // igual que el struct de arriba, se explica solo
    char** intentos; // cantidad de intentos, esta en dinamico por la variabilidad segun dificultad
    int numIntentosRealizados; // cantidad de intentos del usuario
    int largoPalabra;// el largo de la palabra
    bool gano; // flag para ver si gano o no
} PartidaGuardada;

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

//mustra las reglas del wordle
void reglasWordle() ; 

//carga el csv de las tres dificultades principales 
Wordle* cargarCSVWordle(const char *dificultad, int intentos) ;

//carga el csv para el modo infinito , se tiene que cargar dificultad veces si 
void cargarWordleInfinito(Stack* pila, char* dificultad, int intentos) ; 

//muestra el menu del worlde
void menuWordle() ;

//permite jugar una partida normal de wordle 
void juegoNormalWordle(Wordle *partida , int* perdio ) ; 

//le pone datos al wordle 
void ConfigurarWorldes(Wordle **partida, char *opcion, Stack *pilaWordles);

//permite jugar wordle infinito KMS
void jugarInfinito(Stack* pila) ; 

//se explica el nombre, obtiene el numero de la partida, se usa en guardarPartida
int obtener_proximo_numeroP();


//funcion que sirve para guardar las partidas en un archivo :(
void guardarPartida(Wordle* partida, bool gano);

//su nombre lo dice, destruye la partida guardada, se usa para liberar memoria dinamica utilizada
void destruirPG(PartidaGuardada* pg);

//funcion que hace lo mismo que mostrar wordle, pero ahora guarda el intento evaluado y el print de este, para guardarlo en una matriz wordle nueva para el historial.
void reimprimirPG(PartidaGuardada* pg);

//muestra el historial
void mostrarHistorial();

//funcion que resetea el historial, en resumen le pregunta al usuario si quiere resetar sus estadisticas, y si la respuesta es s o S, se borra el archivo para que despues se cree uno nuevo desde 0.
void resetear();


#endif