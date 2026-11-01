#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lexer.h"

bool is_register(TokenType type) {
    return type >= REG_EAX && type <= REG_EDI;
}

void lexer_fill_buffer(Lexer *lexer) {
    size_t read = fread(lexer->buffer, sizeof(char), sizeof(lexer->buffer), lexer->stream);
    if (read < sizeof(lexer->buffer)) {
        lexer->buffer[read] = '\0';
    }
}

void lexer_init(Lexer *lexer, FILE *stream) {
    lexer->buffer_offset = 0;
    lexer->stream = stream;
    lexer_fill_buffer(lexer);
}


char lexer_peek(Lexer *lexer) {
    return lexer->buffer[lexer->buffer_offset];
}

void lexer_advance(Lexer *lexer) {
    lexer->buffer_offset++;
    lexer->buffer_offset %= sizeof(lexer->buffer);
    if (lexer->buffer_offset == 0) {
        lexer_fill_buffer(lexer);
    }
}

bool lexer_is_end(Lexer *lexer) {
    return lexer_peek(lexer) == '\0';
}

bool is_whitespace(char character) {
    return character == ' ' || character == '\n' || character == '\t';
}

bool is_punctuation(char character) {
    return character == ',';
}

bool is_digit(char character) {
    return character >= '0' && character <= '9';
}

Token lexer_read_integer(Lexer *lexer) {
    Token token = { .type = INTEGER };
    size_t index = 0;
    while (is_digit(lexer_peek(lexer))) {
        if (index == TOKEN_SIZE) {
            token.type = ERROR;
            return token;
        }
        token.value[index] = lexer_peek(lexer);
        lexer_advance(lexer);
        index++;
    }
    token.value[index] = '\0';
    return token;
}

Token lexer_read_punctuation(Lexer *lexer) {
    char first = lexer_peek(lexer);
    lexer_advance(lexer);
    switch (first) {
        case ',':
            return (Token) { .type = COMMA, .value = "," };
    }
    return (Token) { .type = ERROR };
}

Token lexer_read_identifier(Lexer *lexer) {
    Token token = { .type = IDENTIFIER };
    size_t index = 0;
    while (!is_whitespace(lexer_peek(lexer)) && !is_punctuation(lexer_peek(lexer))) {
        if (index == TOKEN_SIZE) {
            token.type = ERROR;
            return token;
        }
        token.value[index] = lexer_peek(lexer);
        lexer_advance(lexer);
        index++;
    }
    token.value[index] = '\0';
    
    if (strcmp(token.value, "mov") == 0) {
        token.type = OP_MOV;
        return token;
    }
    if (strcmp(token.value, "syscall") == 0) {
        token.type = OP_SYSCALL;
        return token;
    }
    if (strcmp(token.value, "eax") == 0) {
        token.type = REG_EAX;
        return token;
    }
    if (strcmp(token.value, "edi") == 0) {
        token.type = REG_EDI;
        return token;
    }

    return token;
}

Token lexer_next_token(Lexer *lexer) {
    Token token;
    size_t buffer_pointer = 0;

    while (is_whitespace(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
    
    char first = lexer_peek(lexer);

    if (lexer_is_end(lexer)) {
        token.type = END;
        return token;
    }

    if (is_punctuation(first)) {
        return lexer_read_punctuation(lexer);
    }
    if (is_digit(first)) {
        return lexer_read_integer(lexer);
    }
    return lexer_read_identifier(lexer);
}
