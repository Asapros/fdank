#pragma once

#include <stdbool.h>
#include <stdio.h>

#define TOKEN_SIZE 32
#define LEXER_BUFFER_SIZE 4096

enum {
    IDENTIFIER,
    VALUE,
    MNEMONIC,
    REGISTER,

    COMMA,

    END,
    ERROR,
} typedef TokenType;

enum {
    MOV,
    SYSCALL,

    INVALID_MNEMONIC
} typedef Mnemonic;

struct {
    const char *name;
    const Mnemonic mnemonic;
} typedef MnemonicLookupPair;

static const MnemonicLookupPair MNEMONIC_NAMES[] = {
    { "mov", MOV },
    { "syscall", SYSCALL }
};

enum {
    EAX,
    EDI,

    INVALID_REGISTER
} typedef Register;

struct {
    const char *name;
    const Register reg;
} typedef RegisterLookupPair;

static const RegisterLookupPair REGISTER_NAMES[] = {
    { "eax", EAX },
    { "edi", EDI }
};

enum {
    OVERFLOW
} typedef LexerError;

struct {
    char name[TOKEN_SIZE + 1];
} typedef TokenIdentifierData;

struct {
    long long value;
} typedef TokenValueData;

struct {
    Mnemonic mnemonic;
} typedef TokenMnemonicData;

struct {
    Register reg;
} typedef TokenRegisterData;

struct {
    LexerError error;
} typedef LexerErrorData;

union {
    TokenIdentifierData identifier;
    TokenValueData value;
    TokenMnemonicData mnemonic;
    TokenRegisterData reg;
    LexerErrorData error;
} typedef TokenData;

struct {
    TokenType type;
    TokenData data;
} typedef Token;

struct {
    char buffer[LEXER_BUFFER_SIZE];
    size_t buffer_offset;

    FILE *stream;
} typedef Lexer;

void lexer_init(Lexer *lexer, FILE *stream);
Token lexer_next_token(Lexer *lexer);

Register recognise_register(const char *string);
Mnemonic recognise_mnemonic(const char *string);
