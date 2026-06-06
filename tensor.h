#ifndef TENSOR_H
#define TENSOR_H

#define MAX_DIM 2

#include <stdio.h>      // ha senso fare un header comune per gli include?? capire
#include <sys/types.h>
#include <stdbool.h>

typedef struct {
    int ndim;           // numero di dimensioni del tensore
    int shape[MAX_DIM];     // indica il numero di righe e di colonne
    int size;           // numero di elementi nel tensore (righe*colonne)
    float* data;        // puntatore ai dati
    int ref_count;      // numero di riferimenti al tensore, per decidere se eliminarlo o no
    bool isFilename;    // se vera questo tensore contiene un file name in "data" e in "size" la lunghezza. gli altri campi sono ignorati
    void* map_pointer;  // questo campo contiene il pointer alla mappatura cosi nella free_tensor la si può usare nella munmap
    size_t map_size;    // come sopra, serve per la munmap nel caso il tensore sia inizializzato con '{'
} Tensor;

typedef struct {        // struct per il salvataggio dei file su disco con le operazioni '{' '}'
    int32_t shape[MAX_DIM];
    int32_t ndim;
    off_t data_offset;
} on_disk_tensor;

Tensor* tensor_initialize_from_file(FILE* file);

void truncate_tensor_size(Tensor* tensor);

int tensor_delete(Tensor* tensor);          // dealloca la memoria del tensore

void increment_ref_count(Tensor* tensor);   // +1

void decrement_ref_count(Tensor** tensor);   // -1 e check se è 0, in caso chiamo tensor_delete

void free_tensor(Tensor** tensor);

void tensor_structure_copy(Tensor* t1, Tensor* t2);

void resize_tensor(Tensor* t, int rows, int cols);

bool is_binary_tensor(Tensor* t);

Tensor* tensor_copy(Tensor* t);

#endif