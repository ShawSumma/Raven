#pragma once

#include "raven/compiler/lexer.h"

typedef struct _node {
    Token *value;
    struct _node *left, *right;
} Node;

typedef struct {
    Node *root;
} Ast;

Node *createNode(Token *tok, Node *left, Node *right);
void printNodes(Node *root, int depth);