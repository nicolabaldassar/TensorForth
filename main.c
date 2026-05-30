// Nicola Baldassar SM3201596
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "tensor.h"
#include "operations.h"

void scorri_file(FILE* file, Stack* stack);
void salva_filename(Stack* stack, FILE* file);
void scegli_operazione(Stack* stack, char current);

int main (int argc, char* argv[])
{
    // apertura file e gestione degli errori
    if(argc != 2)
    {
        printf("Errore nel numero di file passati in input.\n");
        exit(EXIT_FAILURE);
    }
    FILE* file = fopen(argv[1], "r");
    if(file == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        exit(EXIT_FAILURE);
    }

    //
    Stack stack = initialize_stack();
    
    // lettura del file
    scorri_file(file, &stack);

    fclose(file);
    free_stack(&stack);
    return 0;
}

// questa funzione serve per alleggerire il main e contiene tutto l'iter per la lettura del file .tensorforth in input
// viene passato il file che stiamo leggendo e lo stack
void scorri_file(FILE* file, Stack* stack)
{
    int current;
    while(1)
    {
        current = fgetc(file);
        if(feof(file))
            break;
        switch(current)
        {
            case '[': {
                Tensor* tensor = tensor_initialize_from_file(file);
                push(stack, tensor);
                break;
            }
            case '"':
                salva_filename(stack, file);
                break;
            case '\n':
                break;
            case ' ':
                break;
            default:
                //operazione
                scegli_operazione(stack, current, filename, filename_dim);
                break;
        }
    }
    return;
}

void salva_filename(Stack* stack, FILE* file)
{
    Tensor* t = malloc(sizeof(Tensor));
    t->size = 256;
    int filename_dim = 0;
    t->data = malloc(sizeof(float) * t->size);
    t->isFilename = true;
    // allochiamo per sicurezza valori non necessari in quanto Filename
    t->ndim = 0;
    t->shape[0] = 0;
    t->shape[1] = 0;
    t->ref_count = 1;
    //
    char current;
    current = fgetc(file);
    while(current != '"' && current != EOF && filename_dim < 255)
    {
        t->data[filename_dim] = (float)current;
        filename_dim++;
        current = fgetc(file);
    }
    t->data[filename_dim] = '\0';
    t->size = filename_dim;
    truncate_tensor_size(t);
    push(stack, t);
}

// questa operazione contiene uno switch e indirizza alle varie funzioni operazioni in base al char letto
// prende in input lo stack e il carattere letto
void scegli_operazione(Stack* stack, char current, char* filename, int filename_dim)
{
    switch(current)
    {
        case 'p':
            print_tens(stack);
            break;
        case '+':
            somma(stack);
            break;
        case '-':
            differenza(stack);
            break;
        case '*':
            prodotto(stack);
            break;
        case '<':
            minore(stack);
            break;
        case '>':
            maggiore(stack);
            break;
        case '=':
            uguale(stack);
            break;
        case 'f':
            fill(stack);
            break;
        case '&':
            and_logico(stack);
            break;
        case '|':
            or_logico(stack);
            break;
        case '!':
            negazione(stack);
            break;
        case '$':
            maschera(stack);
            break;
        case '@':
            matrix_prod(stack);
            break;
        case '.':
            dot_prod(stack);
            break;
        case 'c':
            convoluzione(stack);
            break;
        case 'r':
            reshape(stack);
            break;
        case '_':
            ravel(stack);
            break;
        case '#':
            get_dim(stack);
            break;
        case '?':
            rand_tens(stack);
            break;
        case 'R':
            relu(stack);
            break;
        case 'm':
            min(stack);
            break;
        case 'M':
            max(stack);
            break;
        case 'S':
            somma_tens(stack);
            break;
        case 'd':
            dup(stack);
            break;
        case 's':
            swap(stack);
            break;
        case 'o':
            over(stack);
            break;
        case 'D':
            drop(stack);
            break;
        case '(':
            read_pgm(stack);
            break;
        case ')':
            write_pgm(stack);
            break;
        case '{':
            read_file(stack);
            break;
        case '}':
            write_file(stack);
            break;
        default:
            printf("Errore, carattere sconosciuto.\n");
            exit(EXIT_FAILURE);
            break;
    }
}