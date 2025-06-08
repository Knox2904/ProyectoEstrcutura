#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include "TDAs/list.h"
#include "TDAs/heap.h"
#include "TDAs/extra.h"
#include "kahoot.h"

#define MAX_PREGUNTAS_KAHOOT 10

void menuKahoot() {
    limpiarPantalla();
    puts("========================================");
    puts("     Bienvenido a Kahoot");
    puts("========================================");
    puts("Instrucciones del Kahoot:");
    puts("- Tienes 20 segundos para responder cada pregunta.");
    puts("- Si no respondes a tiempo, se considerará incorrecta.");
    puts("- Solo responderás 10 preguntas por partida.");
    puts("- Preguntas de selección múltiple y verdadero/falso.");
}

void mostrar_pregunta(Pregunta* p, int numero) {
    printf("\nPregunta %d: %s\n", numero + 1, p->pregunta);
    if (p->tipo == MULTIPLE_CHOICE) {
        for (int i = 0; i < 4; i++) {
            if (strlen(p->respuestas[i]) > 0)
                printf("%d) %s\n", i + 1, p->respuestas[i]);
        }
    } else if (p->tipo == TRUE_FALSE) {
        printf("1) Verdadero\n");
        printf("2) Falso\n");
    }
}

int obtener_respuesta_usuario_con_tiempo(Pregunta* p, int* tiempo_usado, bool* respondio) {
    int respuesta = -1;
    time_t inicio = time(NULL);
    printf("\nTu respuesta (tienes %d segundos): ", TIEMPO_LIMITE);
    fflush(stdout);

    char buffer[10] = "";
    int idx = 0;

    while (1) {
        if (_kbhit()) {
            char c = _getch();

            if (c == '\r') {
                buffer[idx] = '\0';
                respuesta = atoi(buffer) - 1;
                *respondio = true;
                break;
            } else if (c >= '0' && c <= '9' && idx < 9) {
                putchar(c);
                buffer[idx++] = c;
            }
        }

        if (difftime(time(NULL), inicio) >= TIEMPO_LIMITE) {
            printf("\n⏰ Tiempo agotado.\n");
            *respondio = false;
            break;
        }

        Sleep(100);
    }

    *tiempo_usado = (int)(time(NULL) - inicio);
    return respuesta;
}

int calcular_puntaje(bool correcta, int tiempo_restante) {
    if (!correcta) return 0;
    return PUNTOS_BASE + (PUNTOS_POR_SEGUNDO * tiempo_restante);
}

void mostrar_resultado_pregunta(bool correcta, int puntos, Pregunta* p) {
    if (correcta) {
        printf("\u2705 ¡Correcto! Has ganado %d puntos.\n", puntos);
    } else {
        printf("\u274c Incorrecto. La respuesta correcta era: %s\n", p->respuestas[p->respuesta_correcta]);
    }
}

void mostrar_resultado_final(Partida* partida) {
    limpiarPantalla();
    puts("===== RESULTADOS DE LA PARTIDA =====");
    printf("Fecha: %s", ctime(&partida->fecha));
    printf("Puntaje total: %d\n", partida->puntaje_total);
    printf("Preguntas correctas: %d/%d\n", partida->preguntas_correctas, partida->total_preguntas);
    printf("Tiempo promedio: %.2f seg\n", partida->tiempo_promedio);
    puts("====================================");
    presioneTeclaParaContinuar();
}

void aleatorizarPreguntas(List* lista) {
    int n = list_size(lista);
    if (n <= 1) return;

    Pregunta** array = malloc(n * sizeof(Pregunta*));
    int i = 0;
    for (Pregunta* p = list_first(lista); p != NULL; p = list_next(lista)) {
        array[i++] = p;
    }

    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Pregunta* temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    list_clean(lista);
    for (int i = 0; i < n; i++) {
        list_pushBack(lista, array[i]);
    }
    free(array);
}

int cargar_preguntas_csv(const char* archivo, List* lista) {
    FILE* file = fopen(archivo, "r");
    if (!file) {
        perror("No se pudo abrir el archivo");
        return 0;
    }

    char linea[512];
    int count = 0;
    fgets(linea, sizeof(linea), file);

    while (fgets(linea, sizeof(linea), file)) {
        Pregunta* p = malloc(sizeof(Pregunta));
        char* token = strtok(linea, ",");
        p->tipo = (strcmp(token, "MULTIPLE_CHOICE") == 0) ? MULTIPLE_CHOICE : TRUE_FALSE;
        strncpy(p->categoria, strtok(NULL, ","), MAX_CATEGORIA);
        strncpy(p->pregunta, strtok(NULL, ","), MAX_PREGUNTA);
        for (int i = 0; i < 4; i++) {
            char* r = strtok(NULL, ",");
            strncpy(p->respuestas[i], r ? r : "", MAX_RESPUESTA);
        }
        p->respuesta_correcta = atoi(strtok(NULL, ","));
        list_pushBack(lista, p);
        count++;
    }
    fclose(file);
    return count;
}

void jugar_kahoot() {
    limpiarPantalla();
    menuKahoot();
    presioneTeclaParaContinuar();

    List* preguntas = list_create();
    int total_disponibles = cargar_preguntas_csv("CSVs/kahoot_40_preguntas.csv", preguntas);
    if (total_disponibles == 0) {
        printf("No se pudieron cargar preguntas.\n");
        return;
    }

    aleatorizarPreguntas(preguntas);

    Partida partida = {0};
    partida.fecha = time(NULL);
    partida.total_preguntas = MAX_PREGUNTAS_KAHOOT;

    time_t inicio_partida = time(NULL);
    int i = 0;
    for (Pregunta* p = list_first(preguntas); p != NULL && i < MAX_PREGUNTAS_KAHOOT; p = list_next(preguntas), i++) {
        limpiarPantalla();
        mostrar_pregunta(p, i);

        int tiempo = 0;
        bool respondio = false;
        int respuesta = obtener_respuesta_usuario_con_tiempo(p, &tiempo, &respondio);

        bool correcta = respondio && (respuesta == p->respuesta_correcta);
        int puntos = calcular_puntaje(correcta, TIEMPO_LIMITE - tiempo);

        partida.puntaje_total += puntos;
        if (correcta) partida.preguntas_correctas++;

        mostrar_resultado_pregunta(correcta, puntos, p);
        presioneTeclaParaContinuar();
    }

    time_t fin_partida = time(NULL);
    partida.tiempo_promedio = (float)(fin_partida - inicio_partida) / partida.total_preguntas;

    mostrar_resultado_final(&partida);
    list_clean(preguntas);
    free(preguntas);
}
