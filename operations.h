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
void minore(Stack* stack);
void maggiore(Stack* stack);
void uguale(Stack* stack);

// operazioni logiche
void and_logico(Stack* stack);
void or_logico(Stack* stack);
void negazione(Stack* stack);

// operazioni di selezione
void maschera(Stack* stack);

// operazione specifiche per i tensori
void matrix_prod(Stack* stack);
void dot_prod(Stack* stack);
void convoluzione(Stack* stack);

// operazioni sulla forme dei tensori
void reshape(Stack* stack);
void ravel(Stack* stack);
void get_dim(Stack* stack);

// operazioni di generazione di numeri casuali
void rand_tens(Stack* stack);

// operazioni elemento per elemento
void relu(Stack* stack);
void min(Stack* stack);
void max(Stack* stack);

// operazioni di riduzione
void somma_tens(Stack* stack);

// operazione di filling di tensori
void fill(Stack* stack);

// operazioni di utilità
void print_tens(Stack* stack);

// operazioni di manipolazione dello stack
void dup(Stack* stack);
void swap(Stack* stack);
void over(Stack* stack);
void drop(Stack* stack);

// I/O operations
void read_pgm(Stack* stack, char* filename);
void write_pgm(Stack* stack, char* filename);
void read_file(Stack* stack, char* filename);
void write_file(Stack* stack, char* filename);




#endif