#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raven/compiler/ast.h"
#include "raven/compiler/lexer.h"
#include "raven/compiler/parser.h"
#include "raven/raven.h"
#include "raven/utils/stack.h"
#include "raven/utils/utils.h"

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
                if (curr->type >= FLOAT_LITERAL) {
                    printf("Line: %d:%d - %f // type: %s\n",
                        curr->loc.line, curr->loc.start,
                        curr->f64, typeToStr(curr->type));
                } else if (curr->type > NUMBER_START) {
                    printf("Line: %d:%d - %d // type: %s\n",
                        curr->loc.line, curr->loc.start,
                        curr->i32, typeToStr(curr->type));
                } else if (curr->type == FUNC_CALL) {
                    printFunc(curr);
                    printf("\n");
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
            Lexer *lexer = createLexer(argv[2]);
            Token *toks = lex(lexer);
            Ast *ast = parse(toks);
            free(ast);
            destroyTokens(toks, 8);
            destroyLexer(lexer);
        }
    }
    return 0;
}
