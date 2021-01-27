#ifdef __cplusplus
extern "C" {
#endif

#include "raven/compiler/lexer.h"
#include "raven/utils/utils.h"

#include <ctype.h>
#include <inttypes.h>
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

static char *allOperators = "+-/*^&|=<>;:";
static char *singleCharBinOps = "+-/*^";
static char *doubleCharBinOps = "+-&|=";
static char *relationOps = "<>";

static bool isAnOperator(char c) {
    for (int i = 0; i < strlen(allOperators); ++i) {
        if (c == allOperators[i]) return true;
    }
    return false;
}

static bool consumeComment(Lexer *lexer);
static bool consumeFunc(Lexer *lexer, Token *t);
static bool consumeIdent(Lexer *lexer, Token *t);
static bool consumeNumber(Lexer *lexer, Token *t);
static bool consumeOperator(Lexer *lexer, Token *t);
static bool consumeString(Lexer *lexer, Token *t);
static bool consumeWhitespace(Lexer *lexer);

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

static void backtrack(Lexer *lex, int n) {
    fseek(lex->file, n, SEEK_CUR);
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
        case FUNC_CALL:
            return "func call";
        case EOF_TOKEN:
            return "eof";
        default:
            break;
    }
    if (tt > NUMBER_START && tt < FLOAT_START) {
        return "integer";
    } else if (tt > FLOAT_START) {
        return "float";
    }
    return "invalid";
}

static bool consumeWhitespace(Lexer *lexer) {
    char c;
    do {
        c = next(lexer);
        if (c == '\n') {
            lexer->currLine++;
            lexer->currCol = 1;
        } else {
            lexer->currCol++;
        }
    } while (isspace(c) && c != EOF);
    backtrack(lexer, -1);
    lexer->currCol--;
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
    lexer->currLine++;
    lexer->currCol = 1;
    if (c == EOF) {
        return true;
    }
    return false;
}

static bool consumeFunc(Lexer *lexer, Token *t) {
    if (consumeWhitespace(lexer)) {
        return true;
    }
    int commaI = 0, argsI = 0;
    int numberOfToksBetweenComma = 5; // how many tokens between ','
    int numberOfArgs = 5; // how many args total
    Token *args = malloc(sizeof(Token) * numberOfArgs);
    Token *currArg = args;
    Token *argsBetweenComma = malloc(sizeof(Token) * numberOfToksBetweenComma);
    Token *currBetween = argsBetweenComma;
    t->function.nargs = 0;
    char c = next(lexer);
    if (c == ')') {
        free(argsBetweenComma);
        free(args);
        args = NULL;
        t->function.args = NULL;
    } else {
        do {
            if (isalnum(c)) {
                backtrack(lexer, -1);
                if (consumeIdent(lexer, currBetween)) {
                    break;
                }
                ++commaI;
                ++currBetween;
            } else if (isdigit(c)) {
                if (consumeNumber(lexer, currBetween)) {
                    break;
                }
                ++commaI;
                ++currBetween;
            } else if (c == '"') {
                if (consumeString(lexer, currBetween)) {
                    break;
                }
                ++commaI;
                ++currBetween;
            } else if (isAnOperator(c)) {
                if (consumeString(lexer, currBetween)) {
                    break;
                }
                ++commaI;
                ++currBetween;
            } else if (c == ',') {
                argsBetweenComma = realloc(argsBetweenComma, sizeof(Token) * commaI);
                *currArg = *argsBetweenComma;
                argsBetweenComma = malloc(sizeof(Token) * numberOfToksBetweenComma);
                currBetween = argsBetweenComma;
                commaI = 0;
                ++currArg;
                ++argsI;
            } else if (c == ')') {
                if (commaI > 0) {
                    argsBetweenComma = realloc(argsBetweenComma, sizeof(Token) * (commaI + 1));
                    *currArg = *argsBetweenComma;
                    argsBetweenComma = malloc(sizeof(Token) * numberOfToksBetweenComma);
                    currBetween = argsBetweenComma;
                    commaI = 0;
                    ++argsI;
                    break;
                }
            }
            if (consumeWhitespace(lexer)) {
                break;
            }
        } while ((c = next(lexer)) != EOF);
    }
    if (args != NULL) {
        args = realloc(args, sizeof(Token) * argsI);
        t->function.args = args;
        t->function.nargs = argsI;
    }
    if (peek(lexer) == EOF) {
        return true;
    }
    return false;
}

static bool consumeIdent(Lexer *lexer, Token *t) {
    char first = next(lexer);
    char c;
    int i = 1; // already have 1 char
    int sizeOfStr = 32;
    char *str = malloc(sizeof(char) * sizeOfStr);
    t->loc.line = lexer->currLine;
    t->loc.start = lexer->currCol; 
    str[0] = first; 
    while (isalnum(c = peek(lexer)) || c == '_') {
        str[i] = c;
        ++i;
        if (i + 1 >= sizeOfStr) {
            sizeOfStr *= 2;
            str = realloc(str, sizeOfStr);
        }
        lexer->currCol++;
        next(lexer);
    }
    t->loc.end = lexer->currCol;
    str[i] = '\0';
    bool found = false;
    for (int i = 0; i < SIZEOF_ARRAY(keywords); ++i) {
        if (strequ(str, keywords[i])) {
            t->type = KEYWORD;
            t->value = str;
            found = true;
            break;
        }
    }
    if (!found) {
        if (consumeWhitespace(lexer)) {
            t->type = IDENT;
            t->value = str;
            return true;
        } else {
            if (peek(lexer) == '(') {
                t->type = FUNC_CALL;
                t->function.name = str;
                next(lexer);
                return consumeFunc(lexer, t);
            } else {
                t->type = IDENT;
                t->value = str;
            }
        }
    }
    if (c == EOF) {
        return true;
    }
    return false;
}

static bool consumeString(Lexer *lexer, Token *t) {
    next(lexer);
    char c;
    int i = 0;
    int sizeOfStr = 32;
    t->loc.line = lexer->currLine;
    t->loc.start = lexer->currCol - 1;
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

static bool consumeOperator(Lexer *lexer, Token *t) {
    char first = next(lexer);
    t->value = malloc(sizeof(char) * 3);
    t->type = INVALID;
    t->loc.line = lexer->currLine;
    t->loc.start = lexer->currCol;
    t->value[0] = first;
    for (int i = 0; i < strlen(singleCharBinOps); ++i) {
        if (first == singleCharBinOps[i]) {
            t->type = BINARY_OP;
            t->value[1] = '\0';
            break;
        }
    }
    for (int i = 0; i < strlen(doubleCharBinOps); ++i) {
        if (first == doubleCharBinOps[i]) {
            char n = peek(lexer);
            if (n == first) {
                t->value[1] = n;
                t->value[2] = '\0';
                next(lexer);
                if (n == '+' || n == '-') {
                    t->type = UNARY_OP;
                    break;
                }
                t->type = BINARY_OP;
                break;
            }
            t->type = BINARY_OP;
            t->value[1] = '\0';
            break;
        }
    }
    for (int i = 0; i < strlen(relationOps); ++i) {
        if (first == relationOps[i]) {
            char n = peek(lexer);
            if (n == '=') {
                t->value[1] = '=';
                t->value[2] = '\0';
                next(lexer);
                t->type = BINARY_OP;
                break;
            }
            t->type = BINARY_OP;
            t->value[1] = '\0';
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
        t->value[1] = '\0';
    }

    if (t->type == INVALID) {
        t->value[1] = '\0';
    }
    t->value = realloc(t->value, sizeof(char) * strlen(t->value));
    return false;
}

static bool consumeNumber(Lexer *lexer, Token *t) {
    char c;
    bool isInt = true;
    int currIdx = 0;
    int typeIdx = 0;
    int sizeOfStr = 32;
    char *str = malloc(sizeof(char) * sizeOfStr);
    char *typeAnnotation = malloc(sizeof(char) * 5);
    t->type = INVALID;
    t->loc.line = lexer->currLine;
    t->loc.start = lexer->currCol;
    while ((c = peek(lexer)) != EOF && (isdigit(c) || c == '.')) {
        if (c == '.') {
            isInt = false;
        }
        str[currIdx] = c;
        ++currIdx;
        lexer->currCol++;
        next(lexer);
    }
    str[currIdx] = '\0';
    if (c != EOF && (c == 'i' || c == 'u' || c == 'f')) {
        next(lexer);
        typeAnnotation[typeIdx] = c;
        ++typeIdx;
        while ((c = peek(lexer)) != EOF && isdigit(c)) {
            typeAnnotation[typeIdx] = c;
            typeIdx++;
            lexer->currCol++;
            next(lexer);
        }
        typeAnnotation[typeIdx] = '\0';

        // TODO: Add handling of invalid definition
        if (strequ(typeAnnotation, "i8")) {
            t->type = INT8_LITERAL;
            if (sscanf(str, "%" SCNd8, &t->i8) < 0) {
                t->type = INVALID;
            }
        } else if (strequ(typeAnnotation, "i16")) {
            t->type = INT16_LITERAL;
            if (sscanf(str, "%" SCNd16, &t->i16) < 0) {
                t->type = INVALID;
            }
        } else if (strequ(typeAnnotation, "i32")) {
            t->type = INT32_LITERAL;
            if (sscanf(str, "%" SCNd32, &t->i32) < 0) {
                t->type = INVALID;
            }
        } else if (strequ(typeAnnotation, "i64")) {
            t->type = INT64_LITERAL;
            if (sscanf(str, "%" SCNd64, &t->i64) < 0) {
                t->type = INVALID;
            }
        } else if (strequ(typeAnnotation, "u8")) {
            t->type = UINT8_LITERAL;
            if (sscanf(str, "%" SCNu8, &t->u8) < 0) {
                t->type = INVALID;
            }
        } else if (strequ(typeAnnotation, "u16")) {
            t->type = UINT16_LITERAL;
            if (sscanf(str, "%" SCNu16, &t->u16) < 0) {
                t->type = INVALID;
            }
        } else if (strequ(typeAnnotation, "u32")) {
            t->type = UINT32_LITERAL;
            if (sscanf(str, "%" SCNu32, &t->u32) < 0) {
                t->type = INVALID;
            }
        } else if (strequ(typeAnnotation, "u64")) {
            t->type = UINT64_LITERAL;
            if (sscanf(str, "%" SCNu64, &t->u64) < 0) {
                t->type = INVALID;
            }
        } else if (strequ(typeAnnotation, "f32")) {
            t->type = FLOAT_LITERAL;
            t->f32 = (float)atof(str);
        } else if (strequ(typeAnnotation, "f64")) {
            t->type = DOUBLE_LITERAL;
            t->f64 = atof(str);
        } else {
            t->type = INVALID;
        }
        if (c == EOF) {
            free(str);
            return true;
        }
    } else {
        if (isInt) {
            t->type = INT32_LITERAL;
            if (sscanf(str, "%" SCNd32, &t->i32) < 0) {
                t->type = INVALID;
            }
        } else {
            t->type = DOUBLE_LITERAL;
            t->f64 = atof(str);
        }
    }
    free(typeAnnotation);
    free(str);
    if (c == EOF) {
        return true;
    }
    return false;
}

Token *lex(Lexer *lexer) {
    int currIdx = 0;
    int numberTokens = 10;
    Token *toks = malloc(sizeof(Token) * numberTokens);
    Token *curr = toks;
    char c;
    consumeWhitespace(lexer);
    while ((c = peek(lexer)) != EOF) {
        bool comment = false;
        if (currIdx >= numberTokens) {
            numberTokens *= 2;
            toks = realloc(toks, sizeof(Token) * numberTokens);
            curr = toks + currIdx;
        }
        TEST_ADD_EOF(consumeWhitespace(lexer), curr, true);
        if (isdigit(c)) {
            TEST_ADD_EOF(consumeNumber(lexer, curr), curr, true);
        } else if (c == '"') {
            TEST_ADD_EOF(consumeString(lexer, curr), curr, true);
        } else if (isalnum(c)) {
            TEST_ADD_EOF(consumeIdent(lexer, curr), curr, true);
        } else if (c == '/') {
            next(lexer);
            if (peek(lexer) == '/') {
                TEST_ADD_EOF(consumeComment(lexer), curr, false);
                comment = true;
            } else {
                backtrack(lexer, -1);
                TEST_ADD_EOF(consumeOperator(lexer, curr), curr, true);
            }
        } else if (isAnOperator(c)) {
            TEST_ADD_EOF(consumeOperator(lexer, curr), curr, true);
        }
        TEST_ADD_EOF(consumeWhitespace(lexer), curr, true);
        if (!comment) {
            ++currIdx;
            ++curr;
        }
    }
    if (!isspace(c)) {
        ++currIdx;
    }
    toks = realloc(toks, sizeof(Token) * currIdx + 1);
    toks[currIdx].type = EOF_TOKEN;
    for (int i = 0; i < currIdx; ++i) {
        toks[i].loc.filename = lexer->filename;
    }
    return toks;
}

Lexer *createLexer(const char *fname) {
    size_t len = strlen(fname);
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->currLine = 1;
    lexer->currCol = 1;
    lexer->filename = malloc(sizeof(char) * len + 1);
    strncpy(lexer->filename, fname, len + 1);
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

static void destroyToken(Token *token) {
    if (token->type < MEM_ALLOCED) {
        free(token->value);
    }
}

void destroyTokens(Token *tokens, size_t n) {
    for (int i = 0; i < n; ++i) {
        destroyToken(tokens + i);
    }
    free(tokens);
}

void printFunc(Token *func) {
    printf("%s(", func->function.name);
        for (int i = 0; i < func->function.nargs; ++i) {
            if (func->function.args[i].type == FUNC_CALL) {
                printFunc(&func->function.args[i]);
            } else {
                printf("%s", func->function.args[i].value);
            }
            if (i + 1 < func->function.nargs) {
                printf(", ");
            }
        }
    printf(")");
}

#ifdef __cplusplus
}
#endif
