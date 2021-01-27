#include "compiler/ast.h"

void astPrintInorder(Node *node) {
    if (node == NULL) return;
    astPrintInorder(node->left);
    printf("%s ", node->value);
    astPrintInorder(node->right);
}
void astPrintPreorder(Node *node) {
    if (node == NULL) return;
    printf("%s ", node->value);
    astPrintPreorder(node->left);
    astPrintPreorder(node->right);
}
void astPrintPostorder(Node *node) {
    if (node == NULL) return;
    astPrintPostorder(node->left);
    astPrintPostorder(node->right);
    printf("%s ", node->value);
}