// Nicola Baldassar SM3201596
#include "tensor.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_TENSOR_SIZE 16

void check_tensor_size(Tensor* tensor, int* temp_size);
void truncate_tensor_size(Tensor* tensor);

// questa funzione va ad inizializzare il tensore
// mode = 0 se vai inizializzato in una dimensione, mode = 1 se va inizializzato a due dimensioni (matrice)
// viene passata la modalità e il file che preso in input dal programma
Tensor* tensor_initialize(int mode, FILE* file)
{
    char current;
    char buffer[256];
    int current_size = 0;
    bool spazio = true;
    int temp_size = INITIAL_TENSOR_SIZE;

    Tensor* tensor = malloc(sizeof(Tensor));    // inizializzo sullo stack cosi poi con la free lo posso liberare nella free_tensor
    tensor->size = 0;
    if(mode == 0) {
        tensor->ndim = 1;
    } else {
        tensor->ndim = 2;
    }
    
    tensor->data = malloc(sizeof(float) * INITIAL_TENSOR_SIZE);
    current = fgetc(file);
    do
    {
        if(spazio == true) {
            if(current_size > 0) {
                tensor->data[tensor->size] = atof(buffer);
                tensor->size = tensor->size + 1;
                check_tensor_size(tensor, &temp_size);
                current_size = 0;
                memset(buffer, 0, sizeof(buffer));  // svuoto il buffer, per usarla importo memset
            }
            if(isspace(current) == 0) {     // se è diverso da " "
                printf("Errore nella forma del tensore.\n");
                exit(EXIT_FAILURE);
            } else {
                spazio = false;
                current = fgetc(file);
            }
        } else {
            if(isspace(current) != 0) {     // se è " "
                printf("Errore nella forma del tensore (doppio spazio!).\n");
                exit(EXIT_FAILURE);
            } else if (isdigit(current) || current == '.') {
                buffer[current_size] = current;
                current_size++;
                //
                current = fgetc(file);
                if(!isdigit(current) && current != '.') {
                    spazio = true;
                }
            }
        }
    } while (current != ']');

    truncate_tensor_size(tensor);

    tensor->ref_count = 1;
    return tensor;
}

// questa funzione viene chiamata ogni volta che si aggiunge un elemento al tensore e fa un check sulla sua dimensione,
// se questa è uguale alla dimensione massima allocata allora va a riallocarlo con spazio doppio
// viene passato il tensore che si sta costruendo e la grandezza attuale
void check_tensor_size(Tensor* tensor, int* temp_size)
{
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
        return;
    }
}

// elimina il tensore, ma prima fa il controllo del ref_count
// prende in input il riferimento al tensore
// output 0 se il tensore è stato eliminato, -1 altrimenti (se il ref_count non lo permette)
int tensor_delete(Tensor* tensor)
{
    if(tensor->ref_count <= 0) {
        free(tensor->data);
        free(tensor);
        tensor->data = NULL;
        tensor = NULL;
        return 0; // eliminato
    }
    return -1; // non eliminato
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
void decrement_ref_count(Tensor* tensor)
{
    if(tensor->ref_count <= 0) {
        tensor_delete(tensor);
    } else {
        tensor->ref_count--;
    }
    return;
}

// questa funzione fa la free e setta a NULL sia il tensor->data che il tensore stesso
// si passa un puntatore al tensore e lo ripulisce completamente liberando tutta la memoria che usa
// bisogna passarli l'indirizzo del tensore &tensor
void free_tensor(Tensor **tensor)
{
    if(tensor == NULL || *tensor == NULL) return;
    free((*tensor)->data);
    (*tensor)->data = NULL;
    free(*tensor); //
    (*tensor) = NULL;
    return;
}