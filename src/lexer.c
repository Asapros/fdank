#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lexer.h"

Register recognise_register(const char *string) {
    for (size_t i = 0; i < sizeof(REGISTER_NAMES) / sizeof(RegisterLookupPair); i++) {
        if (strcmp(string, REGISTER_NAMES[i].name) == 0) return REGISTER_NAMES[i].reg;
    }
    return INVALID_REGISTER;
}

Mnemonic recognise_mnemonic(const char *string) {
    for (size_t i = 0; i < sizeof(MNEMONIC_NAMES) / sizeof(MnemonicLookupPair); i++) {
        if (strcmp(string, MNEMONIC_NAMES[i].name) == 0) return MNEMONIC_NAMES[i].mnemonic;
    }
    return INVALID_MNEMONIC;
}

bool is_mnemonic(const char *string) {
    return (strcmp(string, "mov") == 0)
        || (strcmp(string, "syscall") == 0);
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
    Token token = { .type = VALUE };
    size_t index = 0;
    while (is_digit(lexer_peek(lexer))) {
        token.data.value.value *= 10;
        token.data.value.value += lexer_peek(lexer) - '0';
        // TODO handle overflow
        lexer_advance(lexer);
        index++;
    }
    return token;
}

Token lexer_read_punctuation(Lexer *lexer) {
    char first = lexer_peek(lexer);
    lexer_advance(lexer);
    switch (first) {
        case ',':
            return (Token) { .type = COMMA };
    }
    return (Token) { .type = ERROR };
}

Token lexer_read_identifier(Lexer *lexer) {
    Token token = { .type = IDENTIFIER };
    size_t index = 0;
    while (!is_whitespace(lexer_peek(lexer)) && !is_punctuation(lexer_peek(lexer))) {
        if (index == TOKEN_SIZE) {
            token.type = ERROR;
            token.data.error.error = OVERFLOW;
            return token;
        }
        token.data.identifier.name[index] = lexer_peek(lexer);
        lexer_advance(lexer);
        index++;
    }
    token.data.identifier.name[index] = '\0';
    
    Register reg = recognise_register(token.data.identifier.name);
    if (reg != INVALID_REGISTER) {
        token.type = REGISTER;
        token.data.reg.reg = reg;
        return token;
    }
    Mnemonic mnemonic = recognise_mnemonic(token.data.identifier.name);
    if (mnemonic != INVALID_MNEMONIC) {
        token.type = MNEMONIC;
        token.data.mnemonic.mnemonic = mnemonic;
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
