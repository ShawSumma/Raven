#pragma once

#include "raven/compiler/ast.h"
#include "raven/compiler/lexer.h"
#include "raven/utils/stack.h"
#include "raven/utils/utils.h"

typedef struct stack(Token) TokenStack;
typedef TokenStack OperatorStack;
typedef TokenStack OutputStack;

Ast *parse(Token *tokens);
