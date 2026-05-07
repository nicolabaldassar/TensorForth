// Nicola Baldassar SM3201596
#include <stdio.h>
#include "stack.h"
#include "tensor.h"
#include "operations.h"

int main (int argc, char* argv[])
{
    Stack stack = initialize_stack();
    //
    stack.top = 7;
    Tensor t;
    push(&stack, &t);
    return 0;
}