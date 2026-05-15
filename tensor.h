#ifndef TENSOR_H
#define TENSOR_H

#define DIM 2

#include <stdio.h>      // ha senso fare un header comune per gli include?? capire
#include <sys/types.h>

typedef struct {
    int ndim;           // numero di dimensioni del tensore
    int shape[DIM];     // indica il numero di righe e di colonne
    int size;           // numero di elementi nel tensore (righe*colonne)
    float* data;        // puntatore ai dati
    //FILE* fd;           // puntatore al file da accedere
    int ref_count;      // numero di riferimenti al tensore, per decidere se eliminarlo o no
    //off_t offset;       // per saltare eventuali metadati 
} Tensor;

Tensor tensor_initialize(int mode, FILE* file);

int tensor_delete(Tensor* tensor);          // dealloca la memoria del tensore

void increment_ref_count(Tensor* tensor);    // +1

void decrement_ref_count(Tensor* tensor);    // -1 e check se è 0, in caso chiamo tensor_delete

#endif