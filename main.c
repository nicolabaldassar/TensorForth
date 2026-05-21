// Nicola Baldassar SM3201596
#include <stdio.h>
#include "stack.h"
#include "tensor.h"
#include "operations.h"

void scorri_file(FILE* file, Stack* stack);
int riempi_filename(FILE* file, char* filename);
void scegli_operazione(Stack* stack, char current);

int main (int argc, char* argv[])
{
    // apertura file e gestione degli errori
    if(argc != 2)
    {
        printf("Errore nel numero di file passati in input.\n");
        return -1;
    }
    FILE* file = fopen(argv[1], "r");
    if(file == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        return -1;
    }

    //
    Stack stack = initialize_stack();
    
    // lettura del file
    scorri_file(file, &stack);

    fclose(file);
    return 0;
}

// questa funzione serve per alleggerire il main e contiene tutto l'iter per la lettura del file .tensorforth in input
// viene passato il file che stiamo leggendo e lo stack
void scorri_file(FILE* file, Stack* stack)
{
    char current;
    char filename[256];
    int filename_dim = 0;
    while(1)
    {
        current = fgetc(file);
        if(feof(file))
            break;
        switch(current)
        {
            case '[':
                Tensor* tensor = tensor_initialize_from_file(0, file);
                push(stack, tensor);
                break;
            case '"':
                filename_dim = riempi_filename(file, filename);
                break;
            case '\n':
                break;
            case ' ':
                break;
            default:
                //operazione
                scegli_operazione(stack, current);
                break;
        }
    }
    return;
}

// questa funzione scrive il nome di un file di input/output su una variabile "filename", pronta ad essere usara
// viene chiamata quando viene letto il caratter doppio apice
// prende in input il file da cui stiamo leggendo e il puntatore alla stringa su cui scrivere
int riempi_filename(FILE* file, char* filename)
{
    char current;
    int filename_dim = 0;
    current = fgetc(file);
    do
    {
        filename[filename_dim] = current;
        current = fgetc(file);
        filename_dim++;
    }
    while(current != '"');
    filename[filename_dim] = '\0';
    return filename_dim;
}

// questa operazione contiene uno switch e indirizza alle varie funzioni operazioni in base al char letto
// prende in input lo stack e il carattere letto
void scegli_operazione(Stack* stack, char current)
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
        // case 'm':
        //     min(stack);
        //     break;
        // case 'M':
        //     max(stack);
        //     break;
        default:
            // tornare errrore per char sconosciuto
            break;
    }
}