#include <assert.h>
#include <stdio.h>

#include "raven/compiler/parser.h"

const char *testFile1 = "./test/lexer/test1.txt";
const char *testFile2 = "./test/lexer/test2.txt";


void test1() {
    const Token tokens[9] = {
        {
            .type = KEYWORD,
            .value = "func"
        },
        {
            .type = IDENT,
            .value = "main",
        },
        {
            .type = OPEN_PAREN
        },
        {
            .type = CLOSE_PAREN
        },
        {
            .type = COLON
        },
        {
            .type = IDENT,
            .value = "void",
        },
        {
            .type = OPEN_CPAREN
        },
        {
            .type = CLOSE_CPAREN
        },
        {
            .type = EOF_TOKEN
        }
    };

    Lexer *lexer = createLexer(testFile1);
    assert(lexer != NULL && "Failed to create lexer");
    
    Token *toks = lex(lexer);
    assert(toks != NULL && "Tokens not created correctly");

    for (int i = 0; i < 9; ++i) {
        assert(tokens[i].type == toks->type);
        ++toks;
    }
}

void test2() {
    const Token tokens[21] = {
        {
            .type = KEYWORD,
            .value = "import"
        },
        {
            .type = IDENT,
            .value = "stdio",
        },
        {
            .type = KEYWORD,
            .value = "as"
        },
        {
            .type = IDENT,
            .value = "io"
        },
        {
            .type = SEMICOLON
        },
        {
            .type = KEYWORD,
            .value = "func"
        },
        {
            .type = IDENT,
            .value = "main",
        },
        {
            .type = OPEN_PAREN
        },
        {
            .type = CLOSE_PAREN
        },
        {
            .type = COLON
        },
        {
            .type = IDENT,
            .value = "void",
        },
        {
            .type = OPEN_CPAREN
        },
        {
            .type = IDENT,
            .value = "io"
        },
        {
            .type = BINARY_OP,
            .value = "."
        },
        {
            .type = IDENT,
            .value = "print"
        },
        {
            .type = OPEN_PAREN
        },
        {
            .type = STRING_LITERAL,
            .value = "Hello World!"
        },
        {
            .type = CLOSE_PAREN
        },
        {
            .type = SEMICOLON
        },
        {
            .type = CLOSE_CPAREN
        },
        {
            .type = EOF_TOKEN
        }
    };

    Lexer *lexer = createLexer(testFile2);
    assert(lexer != NULL && "Failed to create lexer");
    
    Token *toks = lex(lexer);
    assert(toks != NULL && "Tokens not created correctly");

    for (int i = 0; i < 21; ++i) {
        assert(tokens[i].type == toks->type);
        ++toks;
    }
}

int main(void) {
    printf("Running Lexer Tests:\n");
    test1();
    printf("test1: success\n");
    test2();
    printf("test2: success\nLexer Tests Successful\n");
    return 0;
}
