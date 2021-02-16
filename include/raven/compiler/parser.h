#pragma once

#include "raven/compiler/ast.h"
#include "raven/compiler/lexer.h"
#include "raven/utils/stack.h"
#include "raven/utils/utils.h"

typedef struct stack(Token) TokenStack;
typedef struct stack(Node) NodeStack;
typedef TokenStack OperatorStack;
typedef NodeStack OutputStack;

Ast *parse(Token *tokens);
