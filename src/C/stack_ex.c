#include <stdio.h>

#include "C/stack.h"
#include "C/log.h"

int main() {
    initLog(stderr);
    Stack stack;
    StackConstruct(&stack, 2);
    StackPush(&stack, 22);
    StackPush(&stack, 22);
    StackPush(&stack, 52223);
    StackPush(&stack, 9823242);
    StackPush(&stack, 124322);
    StackPush(&stack, 122);
    StackPush(&stack, 8626342);

    printf("%lg", StackTop(&stack));
    StackPop(&stack);
    *(((char *) stack.mData) + 20) = 2;
    StackPush(&stack, 1);
}
