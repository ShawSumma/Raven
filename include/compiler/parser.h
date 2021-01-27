#pragma once

#include "ast.h"
#include "lexer.h"
#include "utils/utils.h"

Ast *parse(Token *tokens);