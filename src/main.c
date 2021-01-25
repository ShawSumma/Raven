#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "compiler/lexer.h"
#include "util.h"

int main(int argc, char *argv[]) {
    if (argc == 2) {
        Lexer *lexer = createLexer(argv[1]);
        Token *toks = lex(lexer);
        Token *curr = toks;
        while (curr->type != EOF_TOKEN) {
            if (curr->type > INT_START && curr->type < INT_END) {
                printf("Line: %d:%d - %d // type: %s\n",
                curr->loc.line, curr->loc.start,
                curr->i32, typeToStr(curr->type));
            } else if (curr->type > FLOAT_START && curr->type < FLOAT_END) {
                printf("Line: %d:%d - %.1f // type: %s\n",
                curr->loc.line, curr->loc.start,
                curr->f64, typeToStr(curr->type));
            } else {
                printf("Line: %d:%d - '%s' // type: %s\n",
                    curr->loc.line, curr->loc.start,
                    curr->value, typeToStr(curr->type));
            }
            if (curr->type == INVALID) break;
            ++curr;
        }
        destroyLexer(lexer);
    }
    return 0;
}
