#include "compiler/parser.h"

#include <stdlib.h>

static bool expect(Token *token, TokenType tt) {
    return token->type == tt;
}

static bool isNumber(Token *token) {
    return token->type > INT_START && token->type < FLOAT_END;
}

static bool isOperator(Token *token) {
    return token->type == BINARY_OP || token->type == UNARY_OP;
}

Ast *parse(Token *tokens) {
    Ast *out = malloc(sizeof(Ast));
    return out;
}