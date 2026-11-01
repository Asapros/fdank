#pragma once

#include <stdbool.h>
#include <stdio.h>

#define TOKEN_SIZE 32
#define LEXER_BUFFER_SIZE 4096

enum {
    IDENTIFIER,
    COMMA,
    INTEGER,
    END,
    ERROR,
    OP_MOV,
    OP_SYSCALL,

    REG_EAX,
    REG_EDI

} typedef TokenType;

struct {
    TokenType type;
    char value[TOKEN_SIZE+1];
} typedef Token;

struct {
    char buffer[LEXER_BUFFER_SIZE];
    size_t buffer_offset;

    FILE *stream;
} typedef Lexer;

void lexer_init(Lexer *lexer, FILE *stream);
Token lexer_next_token(Lexer *lexer);

bool is_register(TokenType type);
