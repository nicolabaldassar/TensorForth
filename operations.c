// Nicola Baldassar SM3201596
#include "operations.h"

// stampa il tensore in cima allo stack e lo consuma
// prende in input lo stack
void print_tens(Stack* stack)
{
    Tensor* t = pop(stack);
    printf("[ ");
    for(int i = 0; i < t->size; i++)
    {
        printf("%f ", t->data[i]);
    }
    printf("]\n");
    free_tensor(&t);
    return;
}

// la stampa la somma dei due tensori in cima allo stack
// prende in input lo stack
void somma(Stack* stack)
{
    return;
}