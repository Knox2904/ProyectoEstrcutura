#include "stack.h"
#include <stdlib.h>

Stack* stack_create() {
    return list_create();
}

void stack_push(Stack* stack, void* data) {
    if (stack == NULL) return;
    list_pushFront(stack, data);
}

void* stack_pop(Stack* stack) {
    if (stack == NULL) return NULL;
    return list_popFront(stack);
}

void* stack_top(Stack* stack) {
    if (stack == NULL) return NULL;
    return list_first(stack);
}

int stack_is_empty(Stack* stack) {
    if (stack == NULL) return 1;
    return (list_first(stack) == NULL);
}

int stack_size(Stack* stack) {
    if (stack == NULL) return 0;
    return list_size(stack);
}

void stack_clean(Stack* stack) {
    if (stack == NULL) return;
    list_clean(stack);
}

void stack_destroy(Stack* stack, void (*destroy_data)(void*)) {
    while (!stack_is_empty(stack)) {
        void* data = stack_pop(stack);
        if (destroy_data) {
            destroy_data(data);
        }
    }
    // Aquí liberas la estructura interna de la pila (lista)
    stack_clean(stack);
    free(stack);
}