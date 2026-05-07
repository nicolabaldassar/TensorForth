// Nicola Baldassar SM3201596
#include <stdio.h>
#include "stack.h"
#include "tensor.h"
#include "operations.h"

int main (int argc, char* argv[])
{
    Stack stack = initialize_stack();
    //
    Tensor t;
    t.ndim = 1;
    t.size = 1;
    push(&stack, &t);
    Tensor t2 = peek(&stack);
    printf("%d\n", t2.ndim);
    return 0;
}