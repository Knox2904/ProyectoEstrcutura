#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

#include <stdbool.h>

// Lee un intento del usuario con límite de tiempo.
// Retorna true si se ingresó dentro del tiempo, false si se agotó el tiempo.
// El intento leído se guarda en el buffer entregado.
bool intentoTemporizado(char* buffer, int largoBuffer, int tiempoLimite);

#endif
