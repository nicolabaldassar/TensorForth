// Nicola Baldassar SM3201596
#include "tensor.h"
#include <ctype.h>

//
int tensor_initialize(int mode, FILE* file)
{
    int initial_size = 2;
    char current;
    char buffer[256];
    int current_size = 0;
    bool spazio = true;

    Tensor* tensor;
    if(mode == 0) {
        tensor.ndim = 1;
    } else {
        tensor.ndim = 2;
    }
    
    tensor.data = malloc(sizeof(float) * initial_size);
    do
    {
        current = fgetc(file);

        if(spazio == true) {
            if(current != " ") {
                printf("Errore nella forma del tensore.\n");
                exit(EXIT_FAILURE);
            } else {
                spazio = false;
                break;
            }
        } else {
            if(current == " ") {
                printf("Errore nella forma del tensore (doppio spazio!).\n");
                exit(EXIT_FAILURE);
            } else if (isdigit(current) || current == '.') {
                buffer[current_size] = current;
                current_size++;
            }
        }

        

    } while (current != ']');

    tensor.ref_count = 1;
    return 0;
}