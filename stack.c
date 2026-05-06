// Nicola Baldassar SM3201596
#include <stdlib.h>
#include "stack.h"
#include "tensor.h"

// quando abbiamo una struct normale usiamo il punto se abbiamo puntatore a struct si usa la freccia
Stack* initialize_stack()
{
    int initial_dim = 8;
    Stack stack;
    stack.top = -1;
    // usiamo una dimensione iniziale di 8 puntatori che in caso si va a raddoppiare se lo stack si riempie
    stack.dim = initial_dim;
    stack.tensors_pointer = malloc(sizeof(Tensor*) * stack.dim);
}