// Nicola Baldassar SM3201596
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include "tensor.h"
#define INITIAL_DIM 8

// quando abbiamo una struct normale usiamo il punto se abbiamo puntatore a struct si usa la freccia
Stack initializeStack()
{
    Stack stack;
    stack.top = -1;
    // usiamo una dimensione iniziale di 8 puntatori che in caso si va a raddoppiare se lo stack si riempie
    stack.dim = INITIAL_DIM;
    stack.tensors_pointer = malloc(sizeof(Tensor*) * stack.dim);
    return stack;
}

bool isEmpty(Stack* stack)
{
    if(stack->top <= -1)
        return true;
    else
        return false;
}

bool isFull(Stack* stack)
{
    if(stack->top >= stack->dim - 1)
        return true;
    else
        return false;
}

// 0->rimane come prima; 1->dimezzata: 2->raddoppiata
int resizeStack(Stack* stack)
{
    // controllo se è pieno lo stack, in caso raddoppia
    if(isFull(stack))
    {
        stack->dim *= 2;
        stack->tensors_pointer = realloc(stack->tensors_pointer, sizeof(Tensor*) * stack->dim);
        return 2;
    }
    
    // controllo se è occupato per meno di 1/4, in caso dimezzo
    if(stack->top <= (stack->dim / 4))
    {
        stack->dim = stack->dim / 4;
        stack->tensors_pointer = realloc(stack->tensors_pointer, sizeof(Tensor*) * stack->dim);
        return 1;
    }

    return 0;
}