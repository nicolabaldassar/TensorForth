// Nicola Baldassar SM3201596
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"
#include "tensor.h"
#include "operations.h"

void scorri_file(FILE* file, Stack* stack);
void salva_filename(Stack* stack, FILE* file);
void scegli_operazione(Stack* stack, char current);

// nel main viene aperto il file da leggere, inizializzato lo stack, e chiamata
// la funzione che legge il file elemento per elemento;
// una volta finito di leggere il file questo viene chiuso e lo spazio dello stack liberato.
int main(int argc, char* argv[])
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
    srand(time(NULL));
    // lettura del file
    scorri_file(file, &stack);
    fclose(file);
    free_stack(&stack);
    return 0;
}

// riceve lo stack e il file di input e lo legge elemento per elemento, chiamando le dovute funzioni
// in base al carattere letto
void scorri_file(FILE* file, Stack* stack)
{
    int current;
    // variabile per controllare che ci sia sempre almeno uno spazio fra i token
    bool dopo_token = false;
    while(1)
    {
        current = fgetc(file);
        if(feof(file))
            break;
        switch(current)
        {
            case '[': {
                if(dopo_token) {
                    printf("Errore! Spazio mancante tra i token.\n");
                    exit(EXIT_FAILURE);
                }
                Tensor* tensor = tensor_initialize_from_file(file);
                push(stack, tensor);
                dopo_token = true;
                break;
            }
            case '"':
                if(dopo_token) {
                    printf("Errore! Spazio mancante tra i token.\n");
                    exit(EXIT_FAILURE);
                }
                salva_filename(stack, file);
                dopo_token = true;
                break;
            case '\n':
                dopo_token = false;
                break;
            case ' ':
                dopo_token = false;
                break;
            default:
                if(dopo_token) {
                    printf("Errore! Spazio mancante tra i token.\n");
                    exit(EXIT_FAILURE);
                }
                //operazione
                scegli_operazione(stack, current);
                dopo_token = true;
                break;
        }
    }
    return;
}

// viene chiamata quando il carattere letto sono delle virgolette alte, indicando l'inizio di un
// nome di file che deve essere salvato sullo stack per essere poi utilizzato
void salva_filename(Stack* stack, FILE* file)
{
    Tensor* t = malloc(sizeof(Tensor));
    t->size = 256;
    int filename_dim = 0;
    t->data = malloc(sizeof(float) * t->size);
    t->isFilename = true;
    // allochiamo per sicurezza questi valori, anche se non necessari in quanto Filename
    t->ndim = 0;
    t->shape[0] = 0;
    t->shape[1] = 0;
    t->ref_count = 1;
    t->map_pointer = NULL;
    t->map_size = 0;
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

// questa operazione contiene uno switch e indirizza alle varie funzioni operazioni in base al carattere letto
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