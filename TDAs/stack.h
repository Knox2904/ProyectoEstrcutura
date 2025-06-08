#ifndef STACK_H
#define STACK_H
#include "list.h"

typedef List Stack;

// Funciones básicas de Stack
Stack* stack_create();
void stack_push(Stack* stack, void* data);
void* stack_pop(Stack* stack);
void* stack_top(Stack* stack);
void stack_clean(Stack* stack);

// Funciones auxiliares
int stack_is_empty(Stack* stack);
int stack_size(Stack* stack);

// Cambia la firma para recibir función de destrucción
void stack_destroy(Stack* stack, void (*destroy_data)(void*));

#endif /* STACK_H */