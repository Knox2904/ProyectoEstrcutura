#include <stdio.h> 
#include "TDAs/list.h"
#include "TDAs/queue.h"
#include "funciones/wordle.h"
#include "Temporizador/temporizador.h"
#include "TDAs/extra.h"
#include <windows.h>
#include <time.h>
#include <unistd.h> 
#define MAX_PALABRAS 100
#define MAX_LARGO 32

void menuGeneral() {
  limpiarPantalla();
  puts("========================================");
  puts("     Bienvenido , que desea hacer");
  puts("========================================");

  puts("1) Jugar Kahoot");
  puts("2) Jugar Wordle");
  puts("3) Salir");
}

void menuWordle(){

  limpiarPantalla();
  puts("========================================");
  puts("     Seleccione dificultad");
  puts("========================================");

  puts("1) Facil 🌴");
  puts("2) Normal ⚡");
  puts("3) Extremo 💀");
  puts("4) Salir") ; 


}


Wordle* cargarCSVWordle(char *dificultad, int intentos) {
    char archivo_nombre[50];
    sprintf(archivo_nombre, "CSVs/wordle_%s.csv", dificultad);

    FILE *archivo = fopen(archivo_nombre, "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    char *lista[MAX_PALABRAS];
    int count = 0;
    char buffer[MAX_LARGO];

    while (fgets(buffer, sizeof(buffer), archivo) && count < MAX_PALABRAS) {
        buffer[strcspn(buffer, "\r\n")] = 0; // Quitar salto de línea
        lista[count] = strdup(buffer);
        count++;
    }
    fclose(archivo);

    if (count == 0) {
        fprintf(stderr, "No se encontraron palabras en el CSV.\n");
        return NULL;
    }

    int indice = rand() % count; // o tu función conseguirNumeroAleatorio(count)
    char *palabra = lista[indice];
    int largo = strlen(palabra);

    Wordle *aux = crearWordle(intentos, largo, palabra);

    // Liberar memoria de las palabras no seleccionadas
    for (int i = 0; i < count; i++) {
        if (i != indice) free(lista[i]);
    }

    return aux;
}

void jugarWordle(){
    char opcion ; 
    char* dificultad[] = {"facil", "normal", "dificil"};
    int intentos[] = {6 , 5 , 3} ; 
    Wordle*partida ;
        menuWordle() ; 
        printf("Que desea hacer : \n") ; 
        scanf(" %c" , &opcion) ; 

        switch (opcion){
            case '1' :
                printf("cargando csv\n") ; 
                partida = cargarCSVWordle(dificultad[0] , intentos[0]);
                printf("se cargo correctamente\n") ;
                break;
            case '2' :
                printf("cargando csv\n") ; 
                partida = cargarCSVWordle(dificultad[1] , intentos[1]);
                printf("se cargo correctamente\n") ;
                break; 
                
            case '3' :
                printf("cargando csv\n") ; 
                partida = cargarCSVWordle(dificultad[2] , intentos[2]);
                printf("se cargo correctamente\n") ;
                break;
                
            case '4' :
                break;
            
            default :
                printf("opcion  no valida , selecione una opcion valida (1 o 2)\n") ; 
                break;
        }


        int flag = 1 ;
        int intentosRestantes ; 
        while(1){
        
        intentosRestantes = (partida->maxIntentos - partida->intentoActual) - 1 ; 


        char *intento = malloc(51);

        puts("ingrese su intento : \n") ; 
        getchar() ;
        scanf("%[^\n]", intento);

        flag = strncmp(intento , partida->palabraObjetivo , partida->largoPalabra) ;
        guardarIntento(partida , intento) ; 

        if (flag != 0 && partida->intentoActual < partida->maxIntentos )
        {
            limpiarPantalla() ;
            char resultado[32]; // o usa partida->largoPalabra+1 para mayor seguridad
            evaluarIntento(partida, intento, resultado);
            imprimirResultado(intento, resultado, partida->largoPalabra);
        }



        mostrarWordle(partida) ;
        
        printf("el largo es  : %d\n" , partida->largoPalabra) ; 
        printf("te quedan %d intentos 💀 \n" , intentosRestantes) ; 

            if(flag == 0 || partida->intentoActual >= partida->maxIntentos) {
                printf("la palabra era : %s\n" , partida->palabraObjetivo) ;
                break ; 
            } 
        }


        presioneTeclaParaContinuar();
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    // Semilla única más robusta
    srand((unsigned int)(time(NULL) ^ (getpid() << 16)));

    char opcion;

    do {
        menuGeneral();
        printf("¿Qué desea hacer?: ");
        scanf(" %c", &opcion);

        switch (opcion) {
            case '1':
                // jugarKahoot();
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
        presioneTeclaParaContinuar();
    } while (opcion != '3');

    return 0;
}