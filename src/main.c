#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler/ast.h"
#include "compiler/lexer.h"
#include "raven.h"
#include "utils/stack.h"
#include "utils/utils.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Raven - Version %s\n", RAVEN_VERSION);
        printf("To test lexer: %s --test-lex <filename>\n", argv[0]);
    }
    if (argc == 3) {
        if (strequ(argv[1], "--test-lex")) {
            Lexer *lexer = createLexer(argv[2]);
            Token *toks = lex(lexer);
            Token *curr = toks;
            size_t n = 0;
            while (curr->type != EOF_TOKEN) {
                if (curr->type > INT_START && curr->type < INT_END) {
                    printf("Line: %d:%d - %d // type: %s\n",
                        curr->loc.line, curr->loc.start,
                        curr->i32, typeToStr(curr->type));
                } else if (curr->type > FLOAT_START && curr->type < FLOAT_END) {
                    printf("Line: %d:%d - %f // type: %s\n",
                        curr->loc.line, curr->loc.start,
                        curr->f64, typeToStr(curr->type));
                } else {
                    printf("Line: %d:%d - '%s' // type: %s\n",
                        curr->loc.line, curr->loc.start,
                        curr->value, typeToStr(curr->type));
                }
                ++n;
                ++curr;
            }
            destroyLexer(lexer);
            destroyTokens(toks, n);
        } else if (strequ(argv[1], "--test-parser")) {
            //Lexer *lexer = createLexer(argv[2]);
            //Token *toks = lex(lexer);
             
        }
    }
    if (strequ(argv[1], "--test-stack")) {
        createStack(IntStack, stack, int, 1);
        pushStack(stack, 10);
        pushStack(stack, 20);
        int out;
        popStack(stack, out, -1);
        printf("%d\n", out);
        popStack(stack, out, -1);
        printf("%d\n", out);
        freeStack(stack);
    }
    // if (strequ(argv[1], "--test-ast")) {
    //     Node *n1 = malloc(sizeof(Node));
    //     n1->value = "5";
    //     Node *n2 = malloc(sizeof(Node));
    //     n2->value = "2";
    //     Node *n3 = malloc(sizeof(Node));
    //     n3->value = "1";
    //     Node *n4 = malloc(sizeof(Node));
    //     n4->value = "0";
    //     Node *n5 = malloc(sizeof(Node));
    //     n5->value = "4";
    //     Node *n6 = malloc(sizeof(Node));
    //     n6->value = "3";
    //     Node *n7 = malloc(sizeof(Node));
    //     n7->value = "8";
    //     Node *n8 = malloc(sizeof(Node));
    //     n8->value = "6";
    //     Node *n9 = malloc(sizeof(Node));
    //     n9->value = "7";
    //     Node *n10 = malloc(sizeof(Node));
    //     n10->value = "9";
    //     Node *n11 = malloc(sizeof(Node));
    //     n11->value = "X";

    //     n1->left = n2;
    //     n1->right = n7;
    //     n2->left = n3;
    //     n2->right = n5;
    //     n3->left = n4;
    //     n4->left = n4->right = NULL;
    //     n5->left = n6;
    //     n5->right = NULL;
    //     n6->left = n6->right = NULL;
    //     n7->left = n8;
    //     n7->right = n10;
    //     n8->left = NULL;
    //     n8->right = n9;
    //     n9->left = n9->right = NULL;
    //     n10->left = NULL;
    //     n10->right = n11;
    //     n11->left = n11->right = NULL;

    //     astPrintInorder(n1);
    //     printf("\n");
    //     astPrintPreorder(n1);
    //     printf("\n");
    //     astPrintPostorder(n1);
    //     printf("\n");

    //     free(n1);
    //     free(n2);
    //     free(n3);
    //     free(n4);
    //     free(n5);
    //     free(n6);
    //     free(n7);
    //     free(n8);
    //     free(n9);
    //     free(n10);
    //     free(n11);
    // } else
    return 0;
}
