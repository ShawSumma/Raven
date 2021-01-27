#pragma once

#include <stdlib.h>

#define stack(type)         \
    {                       \
        size_t capacity;    \
        size_t len;         \
        type *objects;      \
    }

#define stackPush(s, obj)                                                   \
    do {                                                                    \
        if (s->len + 1 >= s->capacity) {                                    \
            s->capacity *= 2;                                               \
            s->objects = realloc(s->objects, sizeof(*obj) * self.capacity); \
        }                                                                   \
        s->objects[s->len] = obj;                                           \
        s->len++;                                                           \
    } while(0)

typedef struct stack(int) IntStack;