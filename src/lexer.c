#include "lexer.h"
#include "util.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define TEST_ADD_EOF(p, curr, inc)  \
    if ((p)) {                      \
        if ((inc)) {                \
            ++curr;                 \
        }                           \
        curr->value = "EOF";        \
        curr->type = EOF_TOKEN;     \
        break;                      \
    }

static char *keywords[] = {
    "return", "while", "for", "as",
    "class", "var", "func", "import"
};

static char peek(Lexer *lex) {
    char c = fgetc(lex->file);
    fseek(lex->file, -1, SEEK_CUR);
    return c;
}

static char next(Lexer *lex) {
    return fgetc(lex->file);
}

char *typeToStr(const TokenType tt) {
    switch (tt) {
        case UNARY_OP:
            return "unnary op";
        case BINARY_OP:
            return "binary op";
        case OPEN_PAREN:
            return "open paren";
        case CLOSE_PAREN:
            return "close paren";
        case OPEN_CPAREN:
            return "open cparen";
        case CLOSE_CPAREN:
            return "close cparen";
        case OPEN_SPAREN:
            return "open sparen";
        case CLOSE_SPAREN:
            return "close sparen";
        case COMMA:
            return "comma";
        case COLON:
            return "colon";
        case SEMICOLON:
            return "semicolon";
        case IDENT:
            return "ident";
        case KEYWORD:
            return "keyword";
        case STRING_LITERAL:
            return "string literal";
        case DOC_STRING:
            return "doc string";
        case EOF_TOKEN:
            return "eof";
        default:
            return "invalid";
    }
}

static bool consumeWhitspace(Lexer *lexer) {
    char c;
    do {
        c = next(lexer);
    } while (isspace(c) && c != EOF);
    fseek(lexer->file, -1, SEEK_CUR);
    if (c == EOF) {
        return true;
    }
    return false;
}

static bool consumeComment(Lexer *lexer) {
    char c;
    do {
        c = next(lexer);
    } while (c != '\n' && c != EOF);
    if (c == EOF) {
        return true;
    }
    return false;
}

static bool consumeIdent(Lexer *lexer, Token *t, char first) {
    char c;
    int i = 1; // already have 1 char
    int sizeOfStr = 32;
    char *str = malloc(sizeof(char) * sizeOfStr);
    str[0] = first; 
    while (isalnum(c = next(lexer)) || c == '_') {
        str[i] = c;
        ++i;
        if (i + 1 >= sizeOfStr) {
            sizeOfStr *= 2;
            str = realloc(str, sizeOfStr);
        }
    }
    str[i] = '\0';
    bool found = false;
    for (int i = 0; i < SIZEOF_ARRAY(keywords); ++i) {
        if (strcmp(str, keywords[i]) == 0) {
            t->type = KEYWORD;
            t->value = str;
            found = true;
            break;
        }
    }
    if (!found) {
        t->type = IDENT;
        t->value = str;
    }
    if (c == EOF) {
        return true;
    }
    fseek(lexer->file, -1, SEEK_CUR);
    return false;
}

static bool consumeString(Lexer *lexer, Token *t) {
    char c;
    int i = 0;
    int sizeOfStr = 32;
    char *str = malloc(sizeof(char) * sizeOfStr);
    while (true) {
        c = next(lexer);
        if (c == EOF || c == '\n' || c == '"') {
            break;
        }
        if (c == '\\') {
            c = next(lexer);
            switch (c) {
                case '"':
                    str[i] = '"';
                    break;
                case '\\':
                    str[i] = '\\';
                    break;
                case 'n':
                    str[i] = '\n';
                    break;
                default:
                    str[i] = '\\';
            }
        } else {
            str[i] = c;
        }
        ++i;
        if (i >= sizeOfStr) {
            sizeOfStr *= 2;
            str = realloc(str, sizeOfStr);
        }
    }
    str[i] = '\0';
    t->type = STRING_LITERAL;
    t->value = str;
    if (c == EOF) { 
        return true;
    }
    return false;
}

static char *singleCharBinOps = "+-/*^";
static char *doubleCharBinOps = "+-&|=";
static char *relationOps = "<>";

static bool consumeOperator(Lexer *lexer, Token *t, char first) {
    char *str = malloc(sizeof(char) * 3);
    t->type = INVALID;
    str[0] = first;
    for (int i = 0; i < strlen(singleCharBinOps); ++i) {
        if (first == singleCharBinOps[i]) {
            t->type = BINARY_OP;
            break;
        }
    }
    for (int i = 0; i < strlen(doubleCharBinOps); ++i) {
        if (first == doubleCharBinOps[i]) {
            char n = peek(lexer);
            if (n == first) {
                str[1] = n;
                str[2] = '\0';
                next(lexer);
                if (n == '+' || n == '-') {
                    t->type = UNARY_OP;
                    break;
                }
            }
            t->type = BINARY_OP;
            break;
        }
    }
    for (int i = 0; i < strlen(relationOps); ++i) {
        if (first == relationOps[i]) {
            char n = peek(lexer);
            if (n == '=') {
                str[1] = '=';
                str[2] = '\0';
                next(lexer);
            }
            t->type = BINARY_OP;
            break;
        }
    }
    if (t->type == INVALID) {
        if (first == ',') {
            t->type = COMMA;
        } else if (first == ':') {
            t->type = COMMA;
        } else if (first == ';') {
            t->type = SEMICOLON;
        } else if (first == '(') {
            t->type = OPEN_PAREN;
        } else if (first == ')') {
            t->type = CLOSE_PAREN;
        } else if (first == '{') {
            t->type = OPEN_CPAREN;
        } else if (first == '}') {
            t->type = CLOSE_CPAREN;
        } else if (first == '[') {
            t->type = OPEN_SPAREN;
        } else if (first == ']') {
            t->type = CLOSE_SPAREN;
        }
    }

    if (t->type == INVALID) {
        str[1] = '\0';
    }
    t->value = str;
    return false;
}

Token *lex(Lexer *lexer) {
    int i = 0;
    int numberTokens = 10;
    Token *toks = malloc(sizeof(Token) * numberTokens);
    Token *curr = toks;
    char c;
    consumeWhitspace(lexer);
    while ((c = next(lexer)) != EOF) {
        bool comment = false;
        if (i >= numberTokens) {
            numberTokens *= 2;
            toks = realloc(toks, sizeof(Token) * numberTokens);
            curr = toks + i;
        }

        if (c == EOF) {
            curr->type = EOF_TOKEN;
            break;
        } else if (c == '"') {
            TEST_ADD_EOF(consumeString(lexer, curr), curr, true);
        } else if (isalnum(c)) {
            TEST_ADD_EOF(consumeIdent(lexer, curr, c), curr, true);
        } else if (c == '/' && peek(lexer) == '/') {
            TEST_ADD_EOF(consumeComment(lexer), curr, false);
            comment = true;
        } else {
            TEST_ADD_EOF(consumeOperator(lexer, curr, c), curr, true);
        }
        //printf("%s %d\n", curr->value, curr->type);
        TEST_ADD_EOF(consumeWhitspace(lexer), curr, true);
        if (!comment) {
            ++i;
            ++curr;
        }
    }
    return realloc(toks, sizeof(Token) * i);
}

Lexer *createLexer(const char *fname) {
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->filename = malloc(sizeof(char) * strlen(fname));
    strcpy(lexer->filename, fname);
    lexer->file = fopen(fname, "r");
    if (lexer->file == NULL) {
        fprintf(stderr, "Failed to open file '%s'\n", fname);
        exit(1);
    }
    return lexer;
}

void destroyLexer(Lexer *lex) {
    if (lex != NULL) {
        free(lex->filename);
        lex->filename = NULL;
        fclose(lex->file);
        free(lex);
        lex = NULL;
    }
}