#ifndef STACK_H
#define STACK_H

// serve includere l'header di tensor dato che lo usiamo sotto nei tipi dello stack?????

#include <stdio.h>
#include <stdbool.h>
#include "tensor.h"

typedef struct {
    Tensor* tensors_pointer;                // array di puntatori ai tensori nello stack
    int dim;                                // dimensione array
    int top;                                // posizione elemento superiore dello stack
} Stack;

Stack* initialize_stack();                  // mette top a -1 (stack vuoto)

bool isEmpty(Stack* stack);                 //

bool isFull(Stack* stack);                  //

void push(Stack* stack, Tensor* tensor);    // inserisce un tensore in testa allo stack

Tensor* pop(Stack* stack);                  // ritorna il tensore in testa e lo rimuove dallo stack

Tensor* peek(Stack* stack);                 // ritorna il tensore in testa senza rimuoverlo

#endif