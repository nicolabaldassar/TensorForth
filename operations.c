// Nicola Baldassar SM3201596
#include "operations.h"

// stampa il tensore in cima allo stack, lo stampa e lo consuma
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
}