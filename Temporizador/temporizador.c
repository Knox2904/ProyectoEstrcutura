#include "temporizador.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

bool intentoTemporizado(char* buffer, int largoBuffer, int tiempoLimite) {
    time_t inicio = time(NULL);

    // Leer input del usuario
    fgets(buffer, largoBuffer, stdin);

    time_t fin = time(NULL);
    double tiempoTomado = difftime(fin, inicio);

    // Quitar salto de línea
    buffer[strcspn(buffer, "\n")] = '\0';

    if (tiempoTomado > tiempoLimite) {
        return false;
    }
    return true;
}
