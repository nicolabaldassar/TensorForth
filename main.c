// Nicola Baldassar SM3201596
#include <stdio.h>
#include "stack.h"
#include "tensor.h"
#include "operations.h"

void scorri_file(FILE* file, Stack stack);
int riempi_filename(FILE* file, char* filename);

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
    scorri_file(file, stack);
    
    fclose(file);
    return 0;
}

void scorri_file(FILE* file, Stack stack)
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
                //tensor_initialize(file);
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
                break;
        }
    }
    return;
}

// funzione che salva tutti i caratteri tra due apici in una stringa "filename" pronta per essere consumata
// da un operazione di lettura o di scrittura su file
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