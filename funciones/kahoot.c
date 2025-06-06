#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "TDAs\extra.h"
#include "kahoot.h"

void menuKahoot() 
{
  limpiarPantalla();
  puts("========================================");
  puts("     Bienvenido a Kahoot");
  puts("========================================");

  puts("instrucciones del kahoot");
  puts("contaras con tiempo limitado");
  puts("habaran dos tipos de preguntas que son:");
  puts("pregunta de seleccion multiple y verdadero y falso");
}

void guardarPreguntas(char archivo)
{

}

void mostrar_pregunta(struct Pregunta p, struct pregunta2 k) {
    printf("%s\n", p.pregunta);
    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, p.respuestas[i]);
    }
}

void esCorrecta(char respuesta)
{

}


int calcularPuntaje()
{

    return 0;
}

void aleatorizarPreguntas(struct Pregunta preguntas[], int total_preguntas)
{

}