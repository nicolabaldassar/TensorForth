// Nicola Baldassar SM3201596
#include "stack.h"
#include "tensor.h"

Stack* initialize_stack()
{
    int initial_dim = 8;
    Stack stack;
    stack->top = -1;
    // usiamo una dimensione iniziale di 10 puntatori che in caso si va a raddoppiare se lo stack si riempie
    stack->dim = initial_dim;
    stack->tensor_pointer = malloc(sizeof(Tensor*) * stack->dim);
}