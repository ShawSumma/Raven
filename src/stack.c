#include "utils/stack.h"
#include "utils/utils.h"

Stack *createStack(size_t cap, size_t dataSize) {
    Stack *s = malloc(sizeof(Stack));
    s->capacity = cap;
    s->objects = malloc(dataSize * cap);
    s->size = dataSize;
    return s;
}

void *popStack(Stack *stack) {
    if (stack->len == 0) {
        return NULL;
    }
    stack->len--;
    return stack->objects + (stack->len * stack->size);
}

static void addItemToVoid(void **list, void *obj) {
    (*list) = obj;
}

void pushStack(Stack *stack, void *obj) {
    if (stack->len + 1 >= stack->capacity) {
        stack->capacity *= 2;
        stack->objects = realloc(stack->objects, stack->size * stack->capacity);
    }

    addItemToVoid(, obj);
    stack->len++;
}