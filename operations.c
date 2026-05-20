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
                t3->data[i] = 1.0f;
            } else {
                t3->data[i] = 0.0f;
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
                t3->data[i] = 1.0f;
            } else {
                t3->data[i] = 0.0f;
            }
        }
    }
    push(stack, t3);
    free_tensor(&t1);
    free_tensor(&t2);
    return;   
}

void and_logico(Stack* stack)
{
    Tensor* a = pop(stack);
    Tensor* b = pop(stack);
    Tensor* t = malloc(sizeof(Tensor));
    tensor_structure_copy(t, a);
    if(a->size != b->size) {
        printf("I tensori di un'operazione di comparazione devono essere della stessa dimensione.\n");
        exit(EXIT_FAILURE);
    }
    if(!is_binary_tensor(a) || !is_binary_tensor(b)) {
        printf("I tensori di un'operazione di comparazione devono essere composti solo da 0 e 1.\n");
        exit(EXIT_FAILURE);
    }
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = a->data[i] && b->data[i];
    }
    push(stack, t);
    free_tensor(&a);
    free_tensor(&b);
    return;
}

void or_logico(Stack* stack)
{
    Tensor* a = pop(stack);
    Tensor* b = pop(stack);
    Tensor* t = malloc(sizeof(Tensor));
    tensor_structure_copy(t, a);
    if(a->size != b->size) {
        printf("I tensori di un'operazione di comparazione devono essere della stessa dimensione.\n");
        exit(EXIT_FAILURE);
    }
    if(!is_binary_tensor(a) || !is_binary_tensor(b)) {
        printf("I tensori di un'operazione di comparazione devono essere composti solo da 0 e 1.\n");
        exit(EXIT_FAILURE);
    }
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = a->data[i] || b->data[i];
    }
    push(stack, t);
    free_tensor(&a);
    free_tensor(&b);
    return;
}

void negazione(Stack* stack)
{
    Tensor* a = pop(stack);
    if(!is_binary_tensor(a)) {
        printf("I tensori di un'operazione di comparazione devono essere composti solo da 0 e 1.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    tensor_structure_copy(t, a);
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        if(a->data[i] == 0.0) {
            t->data[i] = 1.0f;
        } else {
            t->data[i] = 0.0f;
        }
    }
    push(stack, t);
    free_tensor(&a);
}

void maschera(Stack* stack)
{
    Tensor* m = pop(stack);
    Tensor* a = pop(stack);
    Tensor* b = pop(stack);
    if(a->size != b->size || b->size != m->size) {
        printf("I tensori devono essere della stessa dimensione.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    tensor_structure_copy(t, m);
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        if(m->data[i] == 1.0) {
            t->data[i] = a->data[i];
        } else {
            t->data[i] = b->data[i];
        }
    }
    push(stack, t);
    free_tensor(&m);
    free_tensor(&a);
    free_tensor(&b);
}

void matrix_prod(Stack* stack)
{
    Tensor* a = pop(stack);
    Tensor* b = pop(stack);
    if(a->ndim != 2 || b->ndim != 2) {
        printf("Per fare un prodotto tra matrici sono richieste delle matrici.\n");
        exit(EXIT_FAILURE);
    }
    if(a->shape[1] != b->shape[0]) {
        printf("Le matrici non hanno la giusta dimensione per fare un prodotto matriciale.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    t->ndim = 2;
    t->shape[0] = a->shape[0];
    t->shape[1] = b->shape[1];
    t->size = t->shape[0] * t->shape[1];
    t->ref_count = 1;
    t->data = malloc(sizeof(float) * t->size);
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = 0.0f;
    }
    #pragma omp parallel for collapse(2)
    for(int i = 0; i < t->shape[0]; i++) {
        for(int j = 0; j < t->shape[1]; j++) {
            for(int k = 0; k < a->shape[1]; k++) {
                t->data[i * t->shape[1] + j] += a->data[i * a->shape[1] + k] * b->data[k * b->shape[1] + j];
            }
        }
    }
    push(stack, t);
    free_tensor(&a);
    free_tensor(&b);
}

void dot_prod(Stack* stack)
{
    Tensor* a = pop(stack);
    Tensor* b = pop(stack);
    if(a->ndim != 1 || b->ndim != 1) {
        printf("Per il prodotto interno i tensori devono essere monodimensionali.\n");
        exit(EXIT_FAILURE);
    }
    if(a->size != b->size) {
        printf("Per il prodotto interno i tensori devono essere della stessa dimensione.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    t->ndim = 1;
    t->shape[0] = 1;
    t->shape[1] = 1;
    t->size = 1;
    t->data = malloc(sizeof(float) * t->size);
    t->ref_count = 1;
    float sum = 0.0f;
    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < a->size; i++) {
        sum += a->data[i] * b->data[i];
    }
    t->data[0] = sum;
    push(stack, t);
    free_tensor(&a);
    free_tensor(&b);
}

void convoluzione(Stack* stack)
{
    Tensor* k = pop(stack);
    Tensor* a = pop(stack);
    if(k->ndim != 2 || a->ndim != 2) {
        printf("I tensori per la convoluzione devono essere in due dimensioni.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* t = malloc(sizeof(Tensor));
    tensor_structure_copy(t, a);
    if(k->shape[0] % 2 == 0 || k->shape[1] % 2 == 0) {
        printf("Il kernel per una convoluzione deve avere dimensioni dispari.\n");
        exit(EXIT_FAILURE);
    }
    // ha senso controllare che k sia > 1? capire
    int x_padding_to_add = (k->shape[1] - 1) / 2;
    int y_padding_to_add = (k->shape[0] - 1) / 2;
    // devo creare il tensore col padding e con a dentro
    Tensor* a_padding = malloc(sizeof(Tensor));
    a_padding->ndim = 2;
    a_padding->shape[0] = a->shape[0] + y_padding_to_add * 2;
    a_padding->shape[1] = a->shape[1] + x_padding_to_add * 2;
    a_padding->size = a_padding->shape[0] * a_padding->shape[1];
    a_padding->ref_count = 1;
    a_padding->data = malloc(sizeof(float) * a_padding->size);
    // riempo la matrice di zeri
    #pragma omp parallel for
    for(int i = 0; i < a_padding->size; i++) {
        a_padding->data[i] = 0;
    }
    // riempio la matrice con a
    #pragma omp parallel for collapse(2)
    for(int i = 0; i < a->shape[0]; i++) {
        for(int j = 0; j < a->shape[1]; j++) {
            int i_ind = i + y_padding_to_add;
            int j_ind = j + x_padding_to_add;
            a_padding->data[i_ind * a_padding->shape[1] + j_ind] = a->data[i * a->shape[1] + j];
        }
    }
    // riempio anche la matrice t di zeri
    #pragma omp parallel for
    for(int i = 0; i < t->size; i++) {
        t->data[i] = 0;
    }
    // faccio il calcolo
    #pragma omp parallel for collapse(2)
    for(int i = 0; i < t->shape[0]; i++) {
        for(int j = 0; j < t->shape[1]; j++) {
            for(int x = 0; x < k->shape[0]; x++) {
                for(int y = 0; y < k->shape[1]; y++) {
                    t->data[i * t->shape[1] + j] += a_padding->data[(i+x) * a_padding->shape[1] + (j+y)] * k->data[x * k->shape[1] + y];
                }
            }
        }
    }
    push(stack, t);
    free_tensor(&a_padding);
    free_tensor(&k);
    free_tensor(&a);
}

void fill(Stack* stack)
{
    Tensor* v = pop(stack);
    Tensor* s = pop(stack);
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