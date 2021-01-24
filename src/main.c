#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "util.h"

int main(int argc, char *argv[]) {
    if (argc == 2) {
        Lexer *lexer = createLexer(argv[1]);
        Token *toks = lex(lexer);
        Token *curr = toks;
        while (curr->type != EOF_TOKEN) {
            ///printf("Line: %d:%d - '%s' // type: %s\n",
            //    curr->loc.line, curr->loc.start,
            //    curr->value, typeToStr(curr->type));
            if (curr->type == INT32_LITERAL) {
                printf("int 32 %d\n", curr->i32);
            } else if (curr->type == UINT64_LITERAL) {
                printf("uint 64 %llu\n", curr->u64);
            } else if (curr->type == FLOAT_LITERAL) {
                printf("float 32 %.1f\n", curr->f32);
            } else if (curr->type == DOUBLE_LITERAL) {
                printf("float 64 %.1f\n", curr->f64); 
            } else {
                //printf("HERE\n");
            }
            ++curr;
        }
        destroyLexer(lexer);
    }
    return 0;
}
