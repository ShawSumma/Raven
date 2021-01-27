#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

typedef enum {
    INVALID = 0,
    // ! - -- + ++
    UNARY_OP,
    // - + * / ^ & && | || . < <= > >= 
    // = += *= /= -= == 
    BINARY_OP,
    OPEN_PAREN,         // (
    CLOSE_PAREN,        // )
    OPEN_CPAREN,        // {
    CLOSE_CPAREN,       // }
    OPEN_SPAREN,        // [
    CLOSE_SPAREN,       // ]
    COMMA,              // ,
    COLON,              // :
    SEMICOLON,          // ;
    IDENT,              
    KEYWORD,
    STRING_LITERAL,     // " "
    DOC_STRING,         /* /// */
    // ###############################
    MEM_ALLOCED,
    // ###############################
    FUNC_CALL,
    CHAR_LITERAL,       // ' '
    EOF_TOKEN,
    NUMBER_START,
    INT8_LITERAL,
    INT16_LITERAL,
    INT32_LITERAL,
    INT64_LITERAL,
    UINT8_LITERAL,
    UINT16_LITERAL,
    UINT32_LITERAL,
    UINT64_LITERAL,
    FLOAT_START,
    FLOAT_LITERAL,
    DOUBLE_LITERAL,
} TokenType;

typedef struct {
    int line;
    int start;
    int end;
    char *filename;
} SourceLoc;

typedef struct _token {
    SourceLoc loc;
    TokenType type;
    union {
        char *value;
        union {
            int8_t      i8;
            int16_t     i16;
            int32_t     i32;
            int64_t     i64;
            uint8_t     u8;
            uint16_t    u16;
            uint32_t    u32;
            uint64_t    u64;
        };
        union {
            float  f32;
            double f64;
        };
        struct _function {
            char *name;
            size_t nargs;
            struct _token *args;
        } function;
    };
} Token;

typedef struct {
    int currLine;
    int currCol;
    char *filename;
    FILE *file;
} Lexer;

char *typeToStr(const TokenType tt);

Token *lex(Lexer *lexer);

Lexer *createLexer(const char *fname);
void destroyLexer(Lexer *lex);

void destroyTokens(Token *tokens, size_t n);

void printFunc(Token *func);

#ifdef __cplusplus
}
#endif
