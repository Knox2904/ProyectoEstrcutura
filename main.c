#include <stdio.h> 
#include "TDAs/list.h"
#include "TDAs/queue.h"
#include "funciones/wordle.h"
#include "funciones/kahoot.h"
#include "Temporizador/temporizador.h"
#include "TDAs/extra.h"
#include <windows.h>
#include <time.h>
#include <unistd.h> 
#include "TDAs/stack.h"
#include <ctype.h>

void menuGeneral() {
  limpiarPantalla();
  puts("========================================");
  puts("              Bienvenido                ");
  puts("========================================");

  puts("1) Jugar Kahoot");
  puts("2) Jugar Wordle");
  puts("3) Salir");
}

void jugarWordle() {
    char opcion = ' '; 
    Wordle *partida = NULL;
    Stack *pilaWordles = stack_create();
    int perdio = 0;

    reglasWordle();
    
    // ConfigurarWorldes ahora maneja todo el menú y la selección.
    
    // La variable 'opcion' se actualizara con tu eleccion(el que ocupe el juego/programa)
    ConfigurarWorldes(&partida, &opcion, pilaWordles);

    // Después de que ConfigurarWorldes termina, decidimos si jugar o no.
    if (opcion >= '1' && opcion <= '3') {
        // Si se eligió un modo normal y se creó una partida
        if (partida != NULL) {
            juegoNormalWordle(partida, &perdio);
            destruirWordle(partida);
        }
    } else if (opcion == '4') {
        // Si se eligió el modo infinito
        jugarInfinito(pilaWordles);
    }
    // Si la opción fue '5' (historial) o '6' (salir), no se hace nada aquí,
    // porque la acción ya se completó dentro de ConfigurarWorldes.
    // La función simplemente termina y vuelve al menú principal.

    // Liberamos la pila por si no se usó (ej: si solo se vio el historial)
    stack_destroy(pilaWordles, NULL);
}


int main() {
    SetConsoleOutputCP(CP_UTF8);

    // Semilla única más robusta
    srand((unsigned int)(time(NULL) ^ (getpid() << 16)));

    char opcion;

    do {
        menuGeneral();
        printf("¿Qué desea hacer?: \n");
        scanf(" %c", &opcion);

        switch (opcion) {
            case '1':
                menuKahoot();
                break;
            case '2':
                jugarWordle();
                break;
            case '3':
                break;
            default:
                printf("Opción no válida. Seleccione una opción válida (1 o 2).\n");
                break;
        }
        
    } while (opcion != '3');

    return 0;
}