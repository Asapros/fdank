#pragma once

#include "lexer.h"
#include "parser.h"
#define MAX_INSTRUCTION_SIZE 15

struct {
    Register reg;
    unsigned char number;
} typedef RegisterNumberLookupPair;

struct {
    unsigned char binary[MAX_INSTRUCTION_SIZE];
    size_t size;
} typedef AssembledBytecode;

AssembledBytecode generate_statement_bytecode(Statement statement);
