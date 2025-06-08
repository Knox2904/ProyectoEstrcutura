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
    char opcion; 
    Wordle *partida = NULL;
    Stack *pilaWordles = stack_create();  // crear pila vacía
    int perdio = 0;

    reglasWordle();      
    menuWordle(); 
    ConfigurarWorldes(&partida, &opcion, pilaWordles);

    if (opcion == '4') {
        jugarInfinito(pilaWordles);
        // Solo destruir pila con datos, ya que cada Wordle está dentro
        stack_destroy(pilaWordles, (void (*)(void *))destruirWordle);
        pilaWordles = NULL;
        // No destruir partida, ya está dentro de la pila
        partida = NULL;
    } 
    else if (opcion != '5') {
        juegoNormalWordle(partida, &perdio);
        // destruir la partida creada normal
        if (partida != NULL) {
            destruirWordle(partida);
            partida = NULL;
        }
        // destruir pila vacía (sin datos)
        stack_destroy(pilaWordles, NULL);
        pilaWordles = NULL;
    } else {
        // opción 5 o salir, liberar todo
        if (partida != NULL) {
            destruirWordle(partida);
            partida = NULL;
        }
        stack_destroy(pilaWordles, NULL);
        pilaWordles = NULL;
    }
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
                jugar_kahoot();
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