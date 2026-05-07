// Nicola Baldassar SM3201596
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include "tensor.h"
#define INITIAL_DIM 8

// quando abbiamo una struct normale usiamo il punto se abbiamo puntatore a struct si usa la freccia
Stack initialize_stack()
{
    Stack stack;
    stack.top = -1;
    // usiamo una dimensione iniziale di 8 puntatori che in caso si va a raddoppiare se lo stack si riempie
    stack.dim = INITIAL_DIM;
    stack.tensors_pointer = malloc(sizeof(Tensor*) * stack.dim);
    return stack;
}

bool is_empty(Stack* stack)
{
    if(stack->top <= -1)
        return true;
    else
        return false;
}

bool is_full(Stack* stack)
{
    if(stack->top >= stack->dim - 1)
        return true;
    else
        return false;
}

// 0->rimane come prima; 1->dimezzata: 2->raddoppiata
int resize_stack(Stack* stack)
{
    // controllo se è pieno lo stack, in caso raddoppia
    if(is_full(stack))
    {
        stack->dim *= 2;
        stack->tensors_pointer = realloc(stack->tensors_pointer, sizeof(Tensor*) * stack->dim);
        printf("    (La dimensione dello stack è aumentata a: %d)\n", stack->dim);
        return 2;
    }
    
    // controllo se è occupato per meno di 1/4, in caso dimezzo
    if(stack->top <= (stack->dim / 4) && stack->dim > INITIAL_DIM)
    {
        stack->dim = stack->dim / 2;
        stack->tensors_pointer = realloc(stack->tensors_pointer, sizeof(Tensor*) * stack->dim);
        printf("    (La dimensione dello stack è diminuita a: %d)\n", stack->dim);
        return 1;
    }

    return 0;
}

void push(Stack* stack, Tensor* tensor)
{
    resize_stack(stack);
    stack->top += 1;
    stack->tensors_pointer[stack->top] = tensor;
}

Tensor pop(Stack* stack)
{
    if(is_empty(stack))
    {
        printf("Errore! Stai provando a leggere un elemento da uno stack vuoto.\n");
        exit(EXIT_FAILURE);
    }
    Tensor t = *stack->tensors_pointer[stack->top];
    stack->top -= 1;
    resize_stack(stack);
    return t;
}

Tensor peek(Stack* stack)
{
    if(is_empty(stack))
    {
        printf("Errore! Stai provando a leggere un elemento da uno stack vuoto.\n");
        exit(EXIT_FAILURE);
    }
    Tensor t = *stack->tensors_pointer[stack->top];
    return t;
}