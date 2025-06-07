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

// respuesta del jugador
typedef struct {
    Pregunta* pregunta;
    int respuesta_usuario;
    int tiempo_respuesta;
    bool correcta;
    int puntos_obtenidos;
} RespuestaUsuario;

// partida completa
typedef struct {
    time_t fecha;
    int puntaje_total;
    int preguntas_correctas;
    int total_preguntas;
    float tiempo_promedio;
} Partida;

// entrada para ranking
typedef struct {
    int puntaje;
    time_t fecha;
    int preguntas_correctas;
    int total_preguntas;
} EntradaRanking;

// funciones principales
void inicializar_kahoot();
void finalizar_kahoot();
void menu_principal();

// funciones de carga de datos
int cargar_preguntas_csv(const char* archivo);
Pregunta* crear_pregunta(char** campos);

// funciones del juego
void jugar_kahoot();
void mostrar_pregunta(Pregunta* p, int numero);
int obtener_respuesta_usuario(Pregunta* p);
int calcular_puntaje(bool correcta, int tiempo_restante);
void mostrar_resultado_pregunta(bool correcta, int puntos, Pregunta* p);
void mostrar_resultado_final(Partida* partida);

// funciones de historial y ranking
void guardar_partida(Partida* partida);
void mostrar_historial();
void mostrar_ranking();
void actualizar_ranking(Partida* partida);

// funciones de comparación para heap y ordenamiento
int comparar_puntajes(void* a, void* b);

// funciones de utilidad and stuff
void mostrar_instrucciones();
int obtener_opcion_menu(int min, int max);
void liberar_pregunta(void* p);

#endif