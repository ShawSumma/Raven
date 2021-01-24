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
            printf("'%s' // type: %s\n", curr->value, typeToStr(curr->type));
            ++curr;
        }
        destroyLexer(lexer);
    }
    return 0;
}
