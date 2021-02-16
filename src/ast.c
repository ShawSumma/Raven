#include "raven/compiler/ast.h"

#include <stdlib.h>

Node *createNode(Token *tok, Node *left, Node *right) {
    Node *n = malloc(sizeof(Node));
    n->value = tok;
    n->left = left;
    n->right = right;
    return n;
}

void printNodes(Node *root, int depth) {
    if (root == NULL) return;

    for (int i = 0; i < depth; ++i) {
        printf(" ");
    }
    if (root->value->type >= FLOAT_LITERAL) {
        printf("| %f\n", root->value->f32);
    } else if (root->value->type > NUMBER_START) {
        printf("| %d\n", root->value->i32);
    } else if (root->value->type != EOF_TOKEN && root->value->type != INVALID) {
        printf("| %s\n", root->value->value);
    } else {
        printf("| other %d\n", root->value->type);
    }

    printNodes(root->left, depth + 1);
    printNodes(root->right, depth + 1);
}