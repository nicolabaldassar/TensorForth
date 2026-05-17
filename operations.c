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

// consuma i due tensori in cima allo stack e ne crea un terzo che è la loro somma
// prende in input lo stack
void somma(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));
    if(t1->size != t2->size) {
        printf("Errore! Stai provando a fare una somma con tensori di dimensione diversa!\n");
        exit(EXIT_FAILURE);
    } else {
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

// consuma i due tensori in cima allo stack e ne crea un terzo che è la loro differenza
// prende in input lo stack
void differenza(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));
    if(t1->size != t2->size) {
        printf("Errore! Stai provando a fare una differenza con tensori di dimensione diversa!\n");
        exit(EXIT_FAILURE);
    } else {
        tensor_structure_copy(t3, t1);
        #pragma omp parallel for
        for(int i = 0; i < t1->size; i++)
        {
            t3->data[i] = t1->data[i] - t2->data[i];
        }
    }
    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
    return;
}

// consuma i due tensori in cima allo stack e ne crea un terzo che è il loro prodotto elemento per elemento
// prende in input lo stack
void prodotto(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));
    if(t1->size != t2->size) {
        printf("Errore! Stai provando a fare un prodotto con tensori di dimensione diversa!\n");
        exit(EXIT_FAILURE);
    } else {
        tensor_structure_copy(t3, t1);
        #pragma omp parallel for
        for(int i = 0; i < t1->size; i++)
        {
            t3->data[i] = t1->data[i] * t2->data[i];
        }
    }
    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
    return;
}

void minore(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));
    if(t1->size != t2->size) {
        printf("Errore! Stai provando a fare una comparazione tra tensori di dimensione diversa!\n");
        exit(EXIT_FAILURE);
    } else {
        tensor_structure_copy(t3, t1);
        #pragma omp parallel for
        for(int i = 0; i < t1->size; i++)
        {
            if(t1->data[i] < t2->data[i]) {
                t3->data[i] = 1.0;
            } else {
                t3->data[i] = 0.0;
            }
        }
    }
    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
    return;   
}

void maggiore(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));
    if(t1->size != t2->size) {
        printf("Errore! Stai provando a fare una comparazione tra tensori di dimensione diversa!\n");
        exit(EXIT_FAILURE);
    } else {
        tensor_structure_copy(t3, t1);
        #pragma omp parallel for
        for(int i = 0; i < t1->size; i++)
        {
            if(t1->data[i] > t2->data[i]) {
                t3->data[i] = 1.0;
            } else {
                t3->data[i] = 0.0;
            }
        }
    }
    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
    return;   
}

void uguale(Stack* stack)
{
    Tensor* t1 = pop(stack);
    Tensor* t2 = pop(stack);
    Tensor* t3 = malloc(sizeof(Tensor));
    if(t1->size != t2->size) {
        printf("Errore! Stai provando a fare una comparazione tra tensori di dimensione diversa!\n");
        exit(EXIT_FAILURE);
    } else {
        tensor_structure_copy(t3, t1);
        #pragma omp parallel for
        for(int i = 0; i < t1->size; i++)
        {
            if(t1->data[i] == t2->data[i]) {
                t3->data[i] = 1.0;
            } else {
                t3->data[i] = 0.0;
            }
        }
    }
    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
    return;   
}

void fill(Stack* stack)
{
    Tensor* v = pop(stack);
    Tensor* s = pop(stack);
    if(v == NULL || s == NULL)
        exit(EXIT_FAILURE);
    if(s->size > 2) {
        printf("Errore! Impossibile inizializzare un tensore di più di 2 dimensioni!\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    if(t == NULL)
        exit(EXIT_FAILURE);
    if(s->size == 1) {
        t->ndim = 1;
        t->shape[0] = 1;
        t->shape[1] = s->data[0];
        t->size = s->data[0];
    } else {
        t->ndim = 2;
        t->shape[0] = s->data[0];
        t->shape[1] = s->data[1];
        t->size = s->data[0] * s->data[1];
    }
    t->data = malloc(sizeof(float) * t->size);
    t->ref_count = 1;
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = v->data[i % v->size];
    }
    push(stack, t);
    free_tensor(&v);
    free_tensor(&s);
}

// se non metto lo spazio finale in un tensore non capta l'errore giusto