
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "wordle.h"
#include "TDAs/extra.h"
#include <windows.h>
#include <unistd.h> 
#include <ctype.h>
#include "TDAs\stack.h"
#define MAX_PALABRAS 50
#define MAX_LARGO 32

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

    
    limpiarPantalla() ; 
    puts("sabes jugar wordle? (y/n)") ;
    char le_sabe ;
    scanf(" %c" , &le_sabe) ; 

    switch (le_sabe) {
    case 'y':
        return;
        break;
    
    case 'n' :
        limpiarPantalla() ; 
        break;    

    default:
        printf("opcion no valida , ingrese [y] para si , o [n] para no \n") ; 
        break;
    }

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

    presioneTeclaParaContinuar() ; 
}

Wordle* cargarCSVWordle(const char *dificultad, int intentos) {
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
    char *palabra = stringToLower(lista[indice]);
    int largo = strlen(palabra);

    Wordle *aux = crearWordle(intentos, largo, palabra);

    // Liberar memoria de las palabras no seleccionadas
    for (int i = 0; i < count; i++) {
        if (i != indice) free(lista[i]);
    }

    return aux;
}

void menuWordle(){

  limpiarPantalla();
  puts("========================================");
  puts("     Seleccione dificultad");
  puts("========================================");

  puts("1) Facil 🌴");
  puts("2) Normal ⚡");
  puts("3) Extremo 💀");
  puts("4) Pruebate a ti mismo 💪") ; 
  puts("5) Salir") ; 


}

void cargarWordleInfinito(Stack* pila, char* dificultad, int intentos) {
    FILE *archivo = fopen("CSVs/wordle_endless.csv", "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return;
    }

    char buffer[MAX_LARGO];
    char *lista[MAX_PALABRAS];
    int count = 0;

    // Leer archivo y filtrar palabras por dificultad
    while (fgets(buffer, sizeof(buffer), archivo) && count < MAX_PALABRAS) {
        buffer[strcspn(buffer, "\r\n")] = 0;

        char *palabra = strtok(buffer, ",");
        char *dif_csv = strtok(NULL, ",");

        if (palabra && dif_csv && strcmp(dif_csv, dificultad) == 0) {
            lista[count++] = strdup(palabra);
        }
    }
    fclose(archivo);

    // Mezclar la lista para aleatoriedad (Fisher-Yates shuffle)
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char *temp = lista[i];
        lista[i] = lista[j];
        lista[j] = temp;
    }

    // Crear Wordles y apilarlos
    for (int i = 0; i < count; i++) {
        char *pal = stringToLower(lista[i]);
        int largo = strlen(pal);

        Wordle *w = crearWordle(intentos, largo, pal);
        stack_push(pila, w);

        free(pal);
        free(lista[i]);
    }
}

void juegoNormalWordle(Wordle *partida , int *perdio){
    *perdio = 0 ; 
    int flag = 1 ;
    printf("el largo es  : %d\n" , partida->largoPalabra) ; 

    while(1){
        
        int intentosRestantes = (partida->maxIntentos - partida->intentoActual) - 1 ; 
        char *intento = malloc(51);

        //se pide la palabra 
        puts("ingrese su intento : \n") ; 
        getchar() ;
        scanf("%[^\n]", intento);

        //es la palabra correcta?
        flag = strncmp(intento , partida->palabraObjetivo , partida->largoPalabra) ;
        guardarIntento(partida , intento) ; 
        

        //es la respuesta incorrecta? , esta dentro de una cantidad valida de intentos?
        if (flag != 0 && partida->intentoActual < partida->maxIntentos )
        {
            limpiarPantalla() ; 
            mostrarWordleColoreado(partida);
            printf("el largo es  : %d\n" , partida->largoPalabra) ;
            printf("te quedan %d intentos 💀 \n" , intentosRestantes) ; 
            
        }

        //se adivino la palabra?
        if (flag == 0 ){
            limpiarPantalla() ; 
            printf("felicidades , encontraste la plabra correcta con %d intentos restantes :) \n" , intentosRestantes) ; 
            break;
        }

        //se quedo sin intentos?
        if(partida->intentoActual >= partida->maxIntentos) {
            limpiarPantalla() ;             
            printf("te quedaste sin intentos , la palabra era : %s\n", partida->palabraObjetivo) ;
            *perdio = 1 ; 
            break ; 
        }
    }

    Sleep(2000);

}

void ConfigurarWorldes(Wordle **partida, char *opcion , Stack* pila) {

    char* dificultad[] = {"facil", "normal", "dificil" , "endless"};
    int intentos[] = {6 , 5 , 3 , 5} ; 

    do {
        printf("Seleccione una opción (1-4) o 5 para salir: ");
        scanf(" %c", opcion); 

        if (*opcion >= '1' && *opcion <= '5') {
            switch (*opcion) {
                case '1':
                    printf("cargando csv\n"); 
                    *partida = cargarCSVWordle(dificultad[0], intentos[0]);
                    printf("se cargo correctamente, iniciando partida\n");
                    Sleep(2000);
                    limpiarPantalla();
                    break;
                case '2':
                    printf("cargando csv\n"); 
                    *partida = cargarCSVWordle(dificultad[1], intentos[1]);
                    printf("se cargo correctamente, iniciando partida\n");
                    Sleep(2000);
                    limpiarPantalla();
                    break;
                case '3':
                    printf("cargando csv\n"); 
                    *partida = cargarCSVWordle(dificultad[2], intentos[2]);
                    printf("se cargo correctamente, suerte 🛐\n");
                    Sleep(2000);
                    limpiarPantalla();
                    break;
                
                case '4':
                    printf("cargando csv\n");
                    for (int i = 2; i >= 0 ; i--) {
                        cargarWordleInfinito(pila, dificultad[i], intentos[i]);
                    }
                    printf("se cargo correctamente el modo infinito, suerte\n");
                    Sleep(2000);
                    limpiarPantalla();
                    break;
                case '5':
                    printf("Saliendo del programa...\n");
                    Sleep(2000);
                    break;
            }
        } else {
            printf("Opción no válida, seleccione una opción válida (1-4) o 5 para salir\n");
        }
    } while (*opcion < '1' || *opcion > '5');
}

void jugarInfinito(Stack* pila) {
    char* dificultad[] = {"facil", "normal", "dificil"};
    int intentos[] = {6, 5, 3};

    if (!pila) {
        printf("Error: pila no inicializada.\n");
        return;
    }

    if (stack_is_empty(pila)) {
        for (int i = 2; i >= 0 ; i--) {
            cargarWordleInfinito(pila, dificultad[i], intentos[i]); 
        }
    }

    printf("Entrando al modo infinito...\n");
    Sleep(1000);
    limpiarPantalla();

    int ronda = 1;
    char continuar = 's';

    while (1) {
        if (stack_is_empty(pila)) {
            printf("Te has quedado sin palabras.\n");
            printf("¿Deseas seguir jugando con nuevas palabras? (s/n): ");
            scanf(" %c", &continuar);

            if (continuar == 's' || continuar == 'S') {
                for (int i = 2; i >= 0 ; i--) {
                    cargarWordleInfinito(pila, dificultad[i], intentos[i]);
                }
                limpiarPantalla();
                continue;
            } else {
                printf("Saliendo del modo infinito.\n");
                break;
            }
        }

        printf("Ronda %d:\n", ronda++);

        Wordle* w = stack_pop(pila);
        if (!w) {
            printf("Error: no se pudo obtener una palabra.\n");
            break;
        }

        int perdio = 0;
        juegoNormalWordle(w, &perdio);
        destruirWordle(w);
        w = NULL;

        if (perdio) {
            printf("Saliendo del modo infinito por derrota.\n");
            break;
        }

        printf("¿Continuar con la siguiente palabra? (s/n): ");
        scanf(" %c", &continuar);
        if (continuar == 'n' || continuar == 'N') {
            printf("Saliendo del modo infinito.\n");
            break;
        }

        limpiarPantalla();
    }

}
