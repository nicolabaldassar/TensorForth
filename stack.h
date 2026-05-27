#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdbool.h>
#include "tensor.h"


typedef struct {
    Tensor** tensors_pointer;               // array di puntatori ai tensori nello stack
    int dim;                                // dimensione array
    int top;                                // posizione elemento superiore dello stack
} Stack;

Stack initialize_stack();                   // mette top a -1 (stack vuoto)

bool is_empty(Stack* stack);                //

bool is_full(Stack* stack);                 //

int resize_stack(Stack* stack);             // controlla se lo stack è pieno lo raddoppia altrimenti se è pieno per meno di 1/4 lo dimezza

void push(Stack* stack, Tensor* tensor);    // inserisce un tensore in testa allo stack

Tensor* pop(Stack* stack);                   // ritorna il tensore in testa e lo rimuove dallo stack

Tensor* peek(Stack* stack);                  // ritorna il tensore in testa senza rimuoverlo

char* pop_filename(Stack* stack);    // prende il filename dallo stack e popola la stringa passata col suo contenuto

#endif