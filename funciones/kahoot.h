#ifndef KAHOOT_H
#define KAHOOT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAX_PREGUNTA 256
#define MAX_RESPUESTA 128
#define MAX_CATEGORIA 64
#define TIEMPO_LIMITE 20 
#define PUNTOS_BASE 1000
#define PUNTOS_POR_SEGUNDO 50

// tipos de preguntas
typedef enum {
    MULTIPLE_CHOICE = 0,
    TRUE_FALSE = 1
} TipoPregunta;

// estructura para una pregunta
typedef struct {
    char pregunta[MAX_PREGUNTA];
    char respuestas[4][MAX_RESPUESTA];  // para multiple 
    int respuesta_correcta;  // 0-3 para multiple, 0-1 para V/F
    TipoPregunta tipo;
    char categoria[MAX_CATEGORIA];
} Pregunta;

// partida completa
typedef struct {
    time_t fecha;
    int puntaje_total;
    int preguntas_correctas;
    int total_preguntas;
    float tiempo_promedio;
} Partida;

// funciones principales
void menuKahoot();

// funciones de carga de datos
int cargar_preguntas_csv(const char* archivo, List* lista);

// funciones del juego
void jugar_kahoot();
void mostrar_pregunta(Pregunta* p, int numero);
int obtener_respuesta_usuario_con_tiempo(Pregunta* p, int* tiempo_usado, bool* respondio);
int calcular_puntaje(bool correcta, int tiempo_restante);
void mostrar_resultado_pregunta(bool correcta, int puntos, Pregunta* p);
void mostrar_resultado_final(Partida* partida);

// funciones de historial y ranking
void mostrar_historial();
void guardar_partida(Partida* partida);
int obtener_opcion_menu(int min, int max);

// funciones de utilidad and stuff
void aleatorizarPreguntas(List* lista);

#endif