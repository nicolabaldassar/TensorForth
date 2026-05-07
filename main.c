// Nicola Baldassar SM3201596
#include <stdio.h>
#include "stack.h"
#include "tensor.h"
#include "operations.h"

int main (int argc, char* argv[])
{
    Stack stack = initializeStack();
    printf("dim: %d\n", stack.dim);
    stack.top = 7;
    int er = resizeStack(&stack);
    printf("dim: %d; la resizeStack ha tornato: %d\n", stack.dim, er);
    return 0;
}