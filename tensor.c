// Nicola Baldassar SM3201596
#include "tensor.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <sys/mman.h>
#define INITIAL_TENSOR_SIZE 16

// forward declaration necessarie per funzioni che sono definite dopo ma presenti in funzioni definite prima
void check_tensor_size(Tensor* tensor, int* temp_size);
void truncate_tensor_size(Tensor* tensor);

// questa funzione va ad inizializzare un tensore dal file di input quando trova il carattere '['
Tensor* tensor_initialize_from_file(FILE* file)
{
    char current;
    char buffer[256];
    int current_size = 0;
    bool spazio = true;
    int temp_size = INITIAL_TENSOR_SIZE;

    Tensor* tensor = malloc(sizeof(Tensor));
    tensor->size = 0;
    
    tensor->data = malloc(sizeof(float) * INITIAL_TENSOR_SIZE);
    current = fgetc(file);
    do
    {
        if(spazio == true) {
            if(current_size > 0) {
                tensor->data[tensor->size] = atof(buffer);
                tensor->size++;
                check_tensor_size(tensor, &temp_size);
                current_size = 0;
                memset(buffer, 0, sizeof(buffer));  // svuoto il buffer se ho scritto il numero corrente
            }
            if(isspace(current) == 0) {  // se è diverso da " "
                printf("Errore nella forma del tensore.\n");
                exit(EXIT_FAILURE);
            } else {
                spazio = false;
            }
        } else {
            while(isspace(current) != 0) {  // se è " "
                current = fgetc(file);
            }
            if(isdigit(current) || current == '.' || current == '-') {
                buffer[current_size] = current;
                current_size++;
                current = fgetc(file);
                if(!isdigit(current) && current != '.' && current != '-') {
                    spazio = true;
                }
            }
        }
    } while(current != ']');
    // errore se manca lo spazio finale

    truncate_tensor_size(tensor);

    tensor->ref_count = 1;
    tensor->isFilename = false;
    tensor->map_pointer = NULL;
    tensor->map_size = 0;
    tensor->ndim = 1;
    tensor->shape[0] = 1;
    tensor->shape[1] = tensor->size;

    return tensor;
}

// questa funzione viene chiamata ogni volta che si aggiunge un elemento al tensore e fa un check sulla sua dimensione,
// se questa è uguale alla dimensione massima allocata allora va a riallocarlo con spazio doppio
// viene passato il tensore che si sta costruendo e la grandezza attuale
void check_tensor_size(Tensor* tensor, int* temp_size)
{
    if(tensor->isFilename) {
        printf("Impossibile eseguire quesa operazione su un filename.\n");
        exit(EXIT_FAILURE);
    }
    if(tensor->size == *temp_size) {
        *temp_size = *temp_size * 2;
        float* temp = realloc(tensor->data, sizeof(float) * (*temp_size));
        if(temp != NULL) {
            tensor->data = temp;
        } else {
            free(tensor->data);
            tensor->data = NULL;
            printf("Errore nel riallocare la memoria.\n");
            return;
        }
    }
}

// questa funzione viene chiamata una volta riempito di valori lo spazio in memoria del tensore e va a eliminare,
// riallocando, tutto lo spazio allocato ma non utilizzato.
// prende in input il tensore su cui si sta lavorando
void truncate_tensor_size(Tensor* tensor) 
{
    float* temp = realloc(tensor->data, sizeof(float) * tensor->size);
    if(temp != NULL) {
        tensor->data = temp;
    } else {
        free(tensor->data);
        printf("Errore nel troncare la dimensione del tensore.\n");
        exit(EXIT_FAILURE);
    }
}


// aumenta il ref count di un tensore
// prende in input il tensore
void increment_ref_count(Tensor* tensor)
{
    tensor->ref_count++;
    return;
}

// decrementa il ref_count e se questo diventa 0 elimina il tensore
// prende in input il riferimento al tensore
// fa quello che fa la free_tensor, infatti la chiama, ma per chiarezza manteniamo due funzioni distinte
// cosi da non confondersi nella lettura del codice
void decrement_ref_count(Tensor** tensor)
{
    free_tensor(tensor);
}

// questa funzione fa la free e setta a NULL sia il tensor->data che il tensore stesso
// si passa un puntatore al tensore e lo ripulisce completamente liberando tutta la memoria che usa
// bisogna passarli l'indirizzo del tensore ( &tensor )
void free_tensor(Tensor **tensor)
{
    if(tensor == NULL || (*tensor) == NULL) return;
    (*tensor)->ref_count--;
    if((*tensor)->ref_count <= 0)
    {
        if((*tensor)->map_pointer != NULL) {
            munmap((*tensor)->map_pointer, (*tensor)->map_size);
        } else {
            free((*tensor)->data);
            (*tensor)->data = NULL;
        }
        free(*tensor);
        (*tensor) = NULL;
    }
}

// questa funzione prende due puntatori a tensori e popola il primo con le informazioni del secondo, 
// tuttavia non copia i dati, ma ne alloca solamente la memoria, questo in visione che i dati saranno
// comunque assegnati successivamente
void tensor_structure_copy(Tensor* t1, Tensor* t2)
{
    if(t1 == NULL || t2 == NULL)    return;
    t1->ndim = t2->ndim;
    t1->shape[0] = t2->shape[0];
    t1->shape[1] = t2->shape[1];
    t1->size = t2->size;
    t1->data = malloc(sizeof(float) * t1->size);
    t1->ref_count = 1;
    t1->isFilename = t2->isFilename;
    t1->map_pointer = NULL;
    t1->map_size = 0;
    return;
}

// questa funzione cambia la forma di un tensore
// viene passato il tensore e il nuovo numero di righe e colonne
// la size deve corrispondere al prodotto di righe per colonne
void resize_tensor(Tensor* t, int rows, int cols)
{
    if(t == NULL)
        exit(EXIT_FAILURE);
    if(t->isFilename) {
        printf("Impossibile eseguire quesa operazione su un filename.\n");
        exit(EXIT_FAILURE);
    }
    if(t->size != rows * cols) {
        printf("Per fare una reshape il nuovo numero di righe e colonne deve corrispondere alla size del tensore.\n");
        exit(EXIT_FAILURE);
    }
    t->ndim = 2;
    t->shape[0] = rows;
    t->shape[1] = cols;
}

// controlla se un tensore è composto solamente da 0 e 1, controllo ricorrente per alcune operazioni
// prende in input il tensore da controllare
// ritorna 0 se non è binario, 1 altrimenti
bool is_binary_tensor(Tensor* t)
{
    if(t == NULL)
        exit(EXIT_FAILURE);
    if(t->isFilename) {
        printf("Impossibile eseguire questa operazione su un filename.\n");
        exit(EXIT_FAILURE);
    }
    int r = 0;
    #pragma omp parallel for reduction(||:r)
    for(int i = 0; i < t->size; i++) {
        if(t->data[i] != 0.0 && t->data[i] != 1.0) {
            r = 1;
        }
    }
    return !r;
}

// ritorna un puntatore a un nuovo tensore completamente identico a quello passato in input
Tensor* tensor_copy(Tensor* t)
{
    if(t->isFilename) {
        printf("Impossibile eseguire quesa operazione su un filename.\n");
        exit(EXIT_FAILURE);
    }
    Tensor* new_tensor = malloc(sizeof(Tensor));
    new_tensor->ndim = t->ndim;
    new_tensor->shape[0] = t->shape[0];
    new_tensor->shape[1] = t->shape[1];
    new_tensor->size = t->size;
    new_tensor->data = malloc(sizeof(float) * new_tensor->size);
    for(int i = 0; i < new_tensor->size; i++) {
        new_tensor->data[i] = t->data[i];
    }
    new_tensor->ref_count = 1;
    new_tensor->isFilename = t->isFilename;
    new_tensor->map_pointer = NULL;
    new_tensor->map_size = 0;
    return new_tensor;
}