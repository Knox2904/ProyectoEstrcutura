#ifndef KAHOOT_H
#define KAHOOT_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

struct Pregunta {
    char pregunta[256];
    char respuestas[4][256];
    int correcta;
};

struct pregunta2 {
    char pregunta[300];
    char verdadero[10];
    char falso[10];
    int respuesta;
};

void menuKahoot();

void mostrar_pregunta(struct Pregunta p, struct pregunta2 k);

void esCorrecta(char respuesta);

int calcularPuntaje();

#endif
