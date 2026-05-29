// Nicola Baldassar SM3201596
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

bool is_empty(Stack* stack);                // controlla se lo stack è vuoto

bool is_full(Stack* stack);                 // controlla se lo stack è pieno

int resize_stack(Stack* stack);             // se lo stack è pieno raddoppia la grandezza, se è pieno per meno di 1/4 la dimezza

void push(Stack* stack, Tensor* tensor);    // inserisce un tensore in testa allo stack

Tensor* pop(Stack* stack);                  // ritorna il tensore in testa allo stack

Tensor* peek(Stack* stack);                 // ritorna il tensore in testa senza rimuoverlo

char* pop_filename(Stack* stack);           // prende il filename dallo stack e popola la stringa passata col suo contenuto

void free_stack(Stack* stack);              // libera la memoria dello stack a fine esecuzione del programma

#endif