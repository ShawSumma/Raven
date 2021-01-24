#pragma once

#include <stdio.h>

typedef enum {
    INVALID = 0,
    // ! - -- + ++
    UNARY_OP,
    // - + * / ^ & && | || . < <= > >= 
    // = += *= /= -= == 
    BINARY_OP,
    OPEN_PAREN, // (
    CLOSE_PAREN, // )
    OPEN_CPAREN, // {
    CLOSE_CPAREN, // }
    OPEN_SPAREN, // [
    CLOSE_SPAREN, // ]
    COMMA,
    COLON,
    SEMICOLON,
    IDENT,
    KEYWORD,
    STRING_LITERAL,
    /* /// */
    DOC_STRING,
    EOF_TOKEN
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

typedef struct {
    char *filename;
    FILE *file;
} Lexer;

char *typeToStr(const TokenType tt);

Token *lex(Lexer *lexer);

Lexer *createLexer(const char *fname);
void destroyLexer(Lexer *lex);
