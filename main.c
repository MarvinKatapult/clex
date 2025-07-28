
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "cvecs.h"

#define MAX_TOKEN_SIZE 1048

#define IS_WHITE_SPACE_TOKEN(C) (C == ' ' || C == '\n' || C == '\t' || c == '\0')
#define IS_OPERATOR(C)     (C == '+' \
                         || C == '-' \
                         || C == '*' \
                         || C == '/' \
                         || C == '(' \
                         || C == ')' \
                         || C == '=' \
                         || C == ';' \
                         || C == ':' \
                         || C == '}' \
                         || C == '{' \
                         || C == ']' \
                         || C == '[' \
                         || C == '~' \
                         || C == '|' \
                         || C == '&' \
                         || C == '!' \
                         || C == '\"' \
                         || C == '\'' \
                         || C == '<' \
                         || C == '>' \
                         || C == '.' \
                         || C == '%' \
                         || C == '?' \
                         || C == '^' \
                         || C == '#' \
                           )

void append_token_and_reset_buffer(char * buf, StrVec * tokens) {
    // printf("Appending Token:>%s<\n", buf);
    appendStrVec(tokens, buf);
    memset(buf, 0, MAX_TOKEN_SIZE);
}

bool tokenize(const char * expression, StrVec * tokens);
bool tokenize_file(const char * filename, StrVec * tokens) {
    FILE * fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Could not open file >%s<\n", filename);
        return false;
    }
    
    char buf[MAX_TOKEN_SIZE];
    while(fgets(buf, sizeof(buf), fp)) {
        if (!tokenize(buf, tokens)) {
            return false;
        }
    }
    fclose(fp);

    return true;
}

bool tokenize(const char * expression, StrVec * tokens) {

    char buf[MAX_TOKEN_SIZE] = { 0 };
    const size_t len = strlen(expression) + 1;
    for (size_t i = 0; i < len; i++) {
        char c = expression[i];
        const bool whitespace = IS_WHITE_SPACE_TOKEN(c);
        const bool operator = IS_OPERATOR(c);
        assert(!(whitespace && operator));

        if (whitespace) {
            if (strlen(buf) != 0) {
                append_token_and_reset_buffer(buf, tokens);
            }
            continue;
        }

        if (operator) {
            if (strlen(buf) != 0) append_token_and_reset_buffer(buf, tokens);
            buf[0] = c;
            append_token_and_reset_buffer(buf, tokens);
            continue;
        }

        buf[strlen(buf)] = c; 
    }

    return true;
}

int main(int argc, char * argv[]) {

    if (argc <= 1) {
        fprintf(stderr, "Usage: %s [Expression]\n", argv[0]);
        return -1;
    }

    printf("Starting to calculate Result of Expression:>%s<\n", argv[1]);

    printf("-- Tokenizing\n");
    StrVec tokens = createStrVec();
    if (!tokenize_file(argv[1], &tokens)) {
        return -1;
    }
    printf("Tokens:\n");
    for (size_t i = 0; i < tokens.count; i++) printf("%s\n", tokens.vals[i]);

    freeStrVec(tokens);
    return 0;
}
