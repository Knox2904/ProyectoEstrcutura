
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

    free(palabra);

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
  puts("5) Ver Historial y Estadisticas 📊");
  puts("6) Resetear Estadísticas 🗑️"); 
  puts("7) Salir") ; 


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
            guardarPartida(partida, true);
            break;
        }

        //se quedo sin intentos?
        if(partida->intentoActual >= partida->maxIntentos) {
            limpiarPantalla() ;             
            printf("te quedaste sin intentos , la palabra era : %s\n", partida->palabraObjetivo) ;
            *perdio = 1 ;
            guardarPartida(partida , false);
            break ; 
        }
    }

    Sleep(2000);

}

void ConfigurarWorldes(Wordle **partida, char *opcion , Stack* pila) {

    char* dificultad[] = {"facil", "normal", "dificil" , "endless"};
    int intentos[] = {6 , 5 , 3 , 5} ; 

    do {

        limpiarPantalla();  // Limpiamos la pantalla para un menu limpio
        menuWordle();      
        
        printf("\nSeleccione una opción: ");
        scanf(" %c", opcion);
        while(getchar() != '\n'); // Limpiar buffer

        // El switch ahora incluye el caso 5 y 6, el 7 es para salir

        if (*opcion >= '1' && *opcion <= '7') {
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
                    mostrarHistorial();
                    break;

                case '6':
                    resetear();
                    break;

                case '7':
                    printf("Saliendo del programa...\n");
                    Sleep(2000);
                    break;
            }
        } else {
            printf("Opción no válida, seleccione una opción válida (1-4) o 5-6 para salir\n");
        }
    } while (*opcion < '1' || *opcion > '6');
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


int obtener_proximo_numeroP() 
{
    // 1. Intenta abrir el archivo en modo lectura.
    FILE* archivo = fopen("CSVs/historial_WordleChad.csv", "r");
    
    // 2. Si el archivo no existe, significa que esta es la primera partida.
    if (!archivo) 
    {
        return 1; 
    }

    char linea[256];
    int ultimo_numero = 0;

    // 3. Lee el archivo línea por línea hasta el final.
    while (fgets(linea, sizeof(linea), archivo)) 
    {
        // 4. Intenta leer un número de una línea que empiece con "NUMERO:".
        
        if (sscanf(linea, "NUMERO:%d", &ultimo_numero) == 1) 
        {
            // Si encuentra un número, actualiza la variable 'ultimo_numero'.
            // Al final del bucle, tendrá el número de la última partida guardada.
        }
    }
    
    fclose(archivo);

    // 5. Devuelve el último número encontrado más uno.
    return ultimo_numero + 1;
}

void guardarPartida(Wordle* partida, bool gano) 
{
    FILE* archivo = fopen("CSVs/historial_WordleChad.csv", "a");
    if (!archivo) 
    {
        perror("No se pudo abrir el historial");
        return;
    }

    int numero_partida = obtener_proximo_numeroP();

    // Se escriben todos los datos necesarios para reconstruir la partida
    fprintf(archivo, "PARTIDA_START\n");
    fprintf(archivo, "NUMERO:%d\n", numero_partida);
    fprintf(archivo, "LARGO_PALABRA:%d\n", partida->largoPalabra);
    fprintf(archivo, "OBJETIVO:%s\n", partida->palabraObjetivo);
    fprintf(archivo, "GANO:%d\n", gano);
    fprintf(archivo, "INTENTOS_REALIZADOS:%d\n", partida->intentoActual);

    // Se guarda cada uno de los intentos
    for (int i = 0; i < partida->intentoActual; i++) {
        fprintf(archivo, "INTENTO:%s\n", partida->TablaWordle[i]);
    }

    fprintf(archivo, "PARTIDA_END\n");
    fclose(archivo);
}

void destruirPG(PartidaGuardada* pg) {
    if (!pg) return;

    free(pg->palabraObjetivo);

    for (int i = 0; i < pg->numIntentosRealizados; i++) {
        free(pg->intentos[i]);
    }
    free(pg->intentos);

    free(pg);
}

void reimprimirPG(PartidaGuardada* pg) {
    printf("\n--- Repetición de la Partida #%d ---\n", pg->numero_partida);
    printf("Palabra objetivo: %s\n\n", pg->palabraObjetivo);

    Wordle evaluador_temp;
    evaluador_temp.largoPalabra = pg->largoPalabra;
    evaluador_temp.palabraObjetivo = pg->palabraObjetivo;

    for (int i = 0; i < pg->numIntentosRealizados; i++) {
        char resultado[MAX_LARGO];
        evaluarIntento(&evaluador_temp, pg->intentos[i], resultado);
        imprimirResultado(pg->intentos[i], resultado, evaluador_temp.largoPalabra);
    }
    printf("--- Fin de la Repetición ---\n");
}

void mostrarHistorial() {
    limpiarPantalla();
    puts("========================================");
    puts("       Historial de Partidas        ");
    puts("========================================");

    // Intenta abrir el archivo de historial para leerlo
    FILE* archivo = fopen("CSVs/historial_WordleChad.csv", "r"); // Usando tu nueva ruta de archivo

    // Si el archivo no se puede abrir, muestra un mensaje y termina.
    if (!archivo) {
        puts("\nNo se ha jugado ninguna partida todavía.");
        presioneTeclaParaContinuar();
        return;
    }
    
    // Si el archivo existe, procede a leerlo.
    Map* historial_map = map_create(int_is_equal);
    char linea[256];
    PartidaGuardada* partida_actual = NULL;
    int contador_intentos = 0;

    // Bucle para leer el archivo línea por línea y construir las estructuras en memoria
    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\r\n")] = 0; // Limpiar saltos de línea

        if (strcmp(linea, "PARTIDA_START") == 0) {
            partida_actual = calloc(1, sizeof(PartidaGuardada));
            contador_intentos = 0;
        } else if (partida_actual) {
            if (sscanf(linea, "NUMERO:%d", &partida_actual->numero_partida) == 1) {}
            else if (sscanf(linea, "LARGO_PALABRA:%d", &partida_actual->largoPalabra) == 1) {}
            else if (strncmp(linea, "OBJETIVO:", 9) == 0) {
                partida_actual->palabraObjetivo = strdup(linea + 9);
            }
            else if (sscanf(linea, "GANO:%d", (int*)&partida_actual->gano) == 1) {}
            else if (sscanf(linea, "INTENTOS_REALIZADOS:%d", &partida_actual->numIntentosRealizados) == 1) {
                partida_actual->intentos = malloc(partida_actual->numIntentosRealizados * sizeof(char*));
            }
            else if (strncmp(linea, "INTENTO:", 8) == 0) {
                if (partida_actual->intentos && contador_intentos < partida_actual->numIntentosRealizados) {
                    partida_actual->intentos[contador_intentos] = strdup(linea + 8);
                    contador_intentos++;
                }
            }
            else if (strcmp(linea, "PARTIDA_END") == 0) {
                int* clave = malloc(sizeof(int));
                *clave = partida_actual->numero_partida;
                map_insert(historial_map, clave, partida_actual);
                partida_actual = NULL;
            }
        }
    }
    fclose(archivo);

    int total_partidas = map_size(historial_map); 
    int partidas_ganadas = 0;
    int suma_intentos_victorias = 0;

    if (total_partidas > 0) {
        MapPair* par_stats = map_first(historial_map);
        while(par_stats) {
            PartidaGuardada* pg = par_stats->value;
            if (pg->gano) {
                partidas_ganadas++;
                suma_intentos_victorias += pg->numIntentosRealizados;
            }
            par_stats = map_next(historial_map);
        }

        float porcentajeV = ((float)partidas_ganadas / total_partidas) * 100.0;
        float promedioI = (partidas_ganadas > 0) ? ((float)suma_intentos_victorias / partidas_ganadas) : 0.0;

        printf("\n ---- Estadísticas Generales ----\n");
        printf("   Partidas Jugadas: %d\n", total_partidas);
        printf("   Porcentaje de Victorias: %.1f%%\n", porcentajeV);
        printf("   Intentos promedio (en victorias): %.2f\n", promedioI);
        printf(" ------------------------------\n");
    }

    // Mostrar el resumen de todas las partidas cargadas
    printf("\nResumen de Partidas Jugadas:\n");
    MapPair* par = map_first(historial_map);
    while(par) {
        PartidaGuardada* pg = par->value;
        printf("Partida #%d: %s en %d intentos. (Palabra: %s)\n",
               pg->numero_partida,
               pg->gano ? "GANADA" : "PERDIDA",
               pg->numIntentosRealizados,
               pg->palabraObjetivo);
        par = map_next(historial_map);
    }

    // Pedir al usuario que elija una partida para verla en detalle
    printf("\nIntroduce el número de partida para ver detalles (0 para salir): ");
    int num_elegido;
    scanf("%d", &num_elegido);
    while (getchar() != '\n');

    
    if (num_elegido > 0) {
        // CAMBIO 1: La variable ahora es de tipo MapPair*
        MapPair* par_elegido = map_search(historial_map, &num_elegido);

        // CAMBIO 2: Se comprueba si el par (la partida) fue encontrado
        if (par_elegido) {
            // CAMBIO 3: Se accede al valor (.value) del par para obtener la partida
            PartidaGuardada* pg_elegida = par_elegido->value;
            reimprimirPG(pg_elegida);
        } else {
            printf("Número de partida no válido.\n");
        }
    }
    
    
    // Bucle final para liberar toda la memoria dinámica utilizada
    par = map_first(historial_map);
    while(par) {
        free(par->key);
        destruirPG(par->value);
        par = map_next(historial_map);
    }
    map_clean(historial_map);
    free(historial_map);

    presioneTeclaParaContinuar();
}

void resetear() 
{
    char confirmacion;
    limpiarPantalla();
    printf("========================================\n");
    printf("      Resetear Historial y Estadísticas\n");
    printf("========================================\n\n");
    printf("ADVERTENCIA: Esta acción borrará permanentemente todo tu historial de partidas.\n");
    printf("¿Estás seguro de que quieres continuar? (s/n): ");

    scanf(" %c", &confirmacion);
    while(getchar() != '\n'); // Limpiar buffer

    if (confirmacion == 's' || confirmacion == 'S') {
        // remove() es una función de la librería estándar <stdio.h>
        if (remove("CSVs/historial_WordleChad.csv") == 0) {
            printf("\nHistorial borrado con éxito.\n");
        } else {
            // Esto puede pasar si el archivo no existía o hay un problema de permisos
            printf("\nNo se pudo borrar el historial (o ya estaba vacío).\n");
        }
    } else {
        printf("\nOperación cancelada. Tu historial está a salvo.\n");
    }

    presioneTeclaParaContinuar();
}
