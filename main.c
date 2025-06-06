#include <stdio.h> 
#include "TDAs/list.h"
#include "TDAs/queue.h"
#include "funciones/wordle.h"
#include "Temporizador/temporizador.h"
#include "TDAs/extra.h"
#include <windows.h>
#include <time.h>
#include <unistd.h> 

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
    // Construir el nombre del archivo CSV dinámicamente
    char archivo_nombre[50]; // Espacio suficiente para el nombre del archivo
    sprintf(archivo_nombre, "CSVs/wordle_%s.csv", dificultad);

    FILE *archivo = fopen(archivo_nombre, "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    char **lista[100]; // Lista para almacenar hasta 100 líneas
    int count = 0;

    // Leer encabezados y descartarlos si no son necesarios
    leer_linea_csv(archivo, ','); 

    while ((lista[count] = leer_linea_csv(archivo, ',')) != NULL) {
        count++;
        if (count >= 100) break;
    }

    fclose(archivo); 

    if (count == 0) {
        fprintf(stderr, "No se encontraron palabras en el CSV.\n");
        return NULL;
    }

    // Generar índice aleatorio seguro dentro del número real de elementos
    int indice = conseguirNumeroAleatorio(count); 


    // Duplicar la palabra correctamente
    char *palabra = strdup(lista[indice][0]); 
    int largo = strlen(palabra); 

    // Crear el objeto Wordle
    Wordle *aux = crearWordle(intentos, largo, palabra); 

    // Mostrar los datos antes de eliminarlos
    printf("Palabras cargadas desde el CSV:\n");
    for (int i = 0; i < count; i++) {
        printf("Fila %d: %s\n", i, lista[i][0]); // Mostrar la palabra de cada fila
    }

    // Liberar memoria usada en lista
    for (int i = 0; i < count; i++) {
        free(lista[i]);
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

        printf("%s\n" , partida->palabraObjetivo) ; 


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