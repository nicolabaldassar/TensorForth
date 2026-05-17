// Nicola Baldassar SM3201596
#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include "stack.h"

// operazioni aritmetiche
void somma(Stack* stack);
void differenza(Stack* stack);
void prodotto(Stack* stack);

// operazioni di comparazione
int minore(Stack* stack);
int maggiore(Stack* stack);
int uguale(Stack* stack);

// operazioni logiche
int and_logico(Stack* stack);
int or_logico(Stack* stack);
int negazione(Stack* stack);

// operazioni di selezione
int maschera(Stack* stack);

// operazione specifiche per i tensori
int matrix_prod(Stack* stack);
int dot_prod(Stack* stack);
int convoluzione(Stack* stack);

// operazioni sulla forme dei tensori
int reshape(Stack* stack);
int ravel(Stack* stack);
int get_dim(Stack* stack);

// operazioni di generazione di numeri casuali
int rand_tens(Stack* stack);

// operazioni elemento per elemento
int relu(Stack* stack);
int min(Stack* stack);
int max(Stack* stack);

// operazioni di riduzione
int somma_tens(Stack* stack);

// operazione di filling di tensori
int fill(Stack* stack);

// operazioni di utilità
void print_tens(Stack* stack);

// operazioni di manipolazione dello stack
int dup(Stack* stack);
int swap(Stack* stack);
int over(Stack* stack);
int drop(Stack* stack);

// I/O operations
int read_pgm(Stack* stack, char* filename);
int write_pgm(Stack* stack, char* filename);
int read_file(Stack* stack, char* filename);
int write_file(Stack* stack, char* filename);




#endif