#pragma once

#include "raven/compiler/lexer.h"

typedef struct _node {
    Token *value;
    struct _node *left, *right;
} Node;

typedef struct {
    Node *root;
} Ast;