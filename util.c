#include "util.h"

void push(Stack* stack, void* value) {
    stack->arr[++stack->top] = value;
}

void* pop(Stack* stack) {
    return stack->arr[stack->top--];
}

void* peek(Stack* stack) {
    return stack->arr[stack->top];
}