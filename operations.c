// Nicola Baldassar SM3201596
#include "operations.h"
#include <stdlib.h>
#include <omp.h>

// stampa il tensore in cima allo stack e lo consuma
// prende in input lo stack
void print_tens(Stack* stack)
{
    Tensor* t = pop(stack);

    printf("Tensor(shape=[%d, %d], data=[ ", t->shape[0], t->shape[1]);
    for(int i = 0; i < t->size; i++)
    {
        printf("%f ", t->data[i]);
    }
    printf("])\n");

    free_tensor(&t);
    return;
}

// la stampa la somma dei due tensori in cima allo stack
// prende in input lo stack
void somma(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));

    if(t1->size != t2->size) {
        printf("Errore! Stai provando a fare una somma con tensori di dimensione diversa!\n");
        return;
    }
    else
    {
        tensor_structure_copy(t3, t1);
        #pragma omp parallel for
        for(int i = 0; i < t1->size; i++)
        {
            t3->data[i] = t1->data[i] + t2->data[i];
        }
    }

    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
    return;
}