#include "raven/compiler/parser.h"

#include <stdlib.h>

/*
static bool expect(Token *token, TokenType tt) {
    return token->type == tt;
}
*/

static bool isNumber(Token *token) {
    return token->type > NUMBER_START;
}

static bool isOperator(Token *token) {
    return token->type == BINARY_OP || token->type == UNARY_OP;
}

static bool isIdent(Token *token) {
    return token->type == IDENT;
}

static bool isString(Token *token) {
    return token->type == STRING_LITERAL;
}

/// + - * /
static uint8_t precedence[] = {
    1, 1, 2, 2
};

static int _plus = 0;
static int _sub = 1;
static int _mult = 2;
static int _div = 3;

static uint8_t getPrecedence(Token *token) {
    if (strequ(token->value, "+")) {
        return precedence[_plus];
    } else if (strequ(token->value, "-")) {
        return precedence[_sub];
    } else if (strequ(token->value, "*")) {
        return precedence[_mult];
    } else if (strequ(token->value, "/")) {
        return precedence[_div];
    }
    return -1;
}

static Token *peekBack(Token *t) {
    return t - 1;
}

static Token *peekForward(Token *t) {
    return t + 1;
}

static Token *parseFuncArgs(Token *t) {

} 

Ast *parse(Token *tokens) {
    Token invalid;
    invalid.type = INVALID;
    Ast *out = malloc(sizeof(Ast));
    createStack(OperatorStack, operatorStack, Token, 10);
    createStack(OutputStack, outputStack, Token, 20);
    Token *curr = tokens;
    int isParen = 0;
    while (curr->type != EOF_TOKEN) {
        if (isOperator(curr)) {
            Token op;
            peekStack(operatorStack, op, invalid);
            if (op.type != INVALID && op.type != OPEN_PAREN && (getPrecedence(curr) <= getPrecedence(&op))) {
                do {
                    popStack(operatorStack, op, invalid);
                    pushStack(outputStack, op);
                    peekStack(operatorStack, op, invalid);
                } while (op.type != INVALID && op.type != OPEN_PAREN && (getPrecedence(curr) <= getPrecedence(&op)));
            }
            pushStack(operatorStack, *curr);
        } else if (curr->type == OPEN_PAREN) {
            pushStack(operatorStack, *curr);
            ++isParen;
        } else if (curr->type == CLOSE_PAREN) {
            if (isParen > 0) {
                Token op;
                do {
                    popStack(operatorStack, op, invalid);
                    if (op.type == OPEN_PAREN) {
                        break;
                    }
                    pushStack(outputStack, op);
                    peekStack(operatorStack, op, invalid);
                } while (op.type != INVALID);
            }

        } else if (isNumber(curr) || isIdent(curr) || isString(curr)) {
            pushStack(outputStack, *curr);
        }
        ++curr;
    }
    Token op;
    popStack(operatorStack, op, invalid);
    do {
        pushStack(outputStack, op);
        popStack(operatorStack, op, invalid);
    } while(op.type != INVALID);

    for (int i = 0; i < outputStack->len; ++i) {
        Token tok = outputStack->objects[i];
        if (tok.type > FLOAT_START) {
            printf("%f ", tok.f64);
        } else if (tok.type > NUMBER_START) {
            printf("%d ", tok.i32);
        } else {
            printf("%s ", tok.value);
        }
    }
    printf("\n");
    return out;
}