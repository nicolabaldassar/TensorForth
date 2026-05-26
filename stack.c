// Nicola Baldassar SM3201596
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include "tensor.h"
#define INITIAL_DIM 8


// questa funzione viene chiamata nel main per creare e inizializzare lo stack
// non ha input, torna un riferimento allo stack inizializzato
Stack initialize_stack()
{
    Stack stack;
    stack.top = -1;
    stack.dim = INITIAL_DIM;
    stack.tensors_pointer = malloc(sizeof(Tensor*) * stack.dim);
    return stack;
}

// ritorna true se lo stack è vuoto, false altrimenti
// viene passato in input lo stack
bool is_empty(Stack* stack)
{
    if(stack->top <= -1)
        return true;
    else
        return false;
}

// ritorna true se lo stack è pieno, false altrimenti
// viene passato in input lo stack
bool is_full(Stack* stack)
{
    if(stack->top >= stack->dim - 1)
        return true;
    else
        return false;
}

// questa funzione fa un controllo sulla dimensione dello stack e agisce di conseguenza
// se lo stack è pieno lo raddoppia, se è pieno per meno di 1/4 lo dimezza, non fa nulla altrimenti
// ritorna 0 se non fa nulla, 1 se dimezza lo stack, 2 se lo raddoppia
// prende in input lo stack
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

// funzione che carica un tensore nello stack
// viene passato lo stack e il tensore da ricaricare
void push(Stack* stack, Tensor* tensor)
{
    resize_stack(stack);
    stack->top += 1;
    stack->tensors_pointer[stack->top] = tensor;
}

// funzione che estrae il tensore più in testa allo stack e decrementa l'indice del tensore in cima allo stack
// tuttavia non lo elimina dallo stack, questo sarà compito di chiama la pop()
// viene passato lo stack
Tensor* pop(Stack* stack)
{
    if(is_empty(stack))
    {
        printf("Errore! Stai provando a leggere un elemento da uno stack vuoto.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = stack->tensors_pointer[stack->top];
    if(t == NULL) {
        printf("Errore! Il tensore sullo stack è NULL;\n");
        exit(EXIT_FAILURE);
    }
    if(t->ref_count > 1) {
        Tensor* t_copy = tensor_copy(t);
        t->ref_count--;
        stack->top--;
        resize_stack(stack);
        return t_copy;
    }
    t->ref_count--;
    stack->top--;
    resize_stack(stack);
    return t;
}

// funzione che estrae il valore in testa allo stack ma senza decrementare l'indice del tensore in testa
// quindi richiamando la peek/pop viene ripescato lo stesso tensore
// viene passato lo stack
Tensor* peek(Stack* stack)
{
    if(is_empty(stack))
    {
        printf("Errore! Stai provando a leggere un elemento da uno stack vuoto.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = stack->tensors_pointer[stack->top];
    return t;
}