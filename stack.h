// Nicola Baldassar SM3201596
#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdbool.h>
#include "tensor.h"

// lo stack è formato da un array di puntatori a puntatori a tensori
// contiene la dimensione corrente dello stack e la posizione del tensore più in cima
typedef struct {
    Tensor** tensors_pointer;
    int dim;
    int top;
} Stack;

Stack initialize_stack();

bool is_empty(Stack* stack);

bool is_full(Stack* stack);

void resize_stack(Stack* stack);

void push(Stack* stack, Tensor* tensor;

Tensor* pop(Stack* stack);

Tensor* peek(Stack* stack);

char* pop_filename(Stack* stack);

void free_stack(Stack* stack);

#endif