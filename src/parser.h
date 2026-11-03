#pragma once

#include "lexer.h"

#define PARSER_BUFFER_SIZE 8

struct {
    Token token;
} typedef ParsingError;

enum {
    OPERAND_REGISTER,
    OPERAND_VALUE,

    OPERAND_INVALID
} typedef OperandType;

struct {
    Register reg;
} typedef OperandRegisterData;

struct {
    long long value;
} typedef OperandValueData;

union {
    OperandRegisterData reg;
    OperandValueData value;
    ParsingError error;

} typedef OperandData;

struct {
    OperandType type;
    OperandData data;
} typedef Operand;

enum {
    STATEMENT_INSTRUCTION,

    STATEMENT_END,
    STATEMENT_INVALID
} typedef StatementType;

struct {
    Mnemonic mnemonic;
    Operand operands[3];
    size_t operand_count;
} typedef StatementInstructionData;

union {
    StatementInstructionData instruction;
    ParsingError error;
} typedef StatementData;

struct {
    StatementType type;
    StatementData data;
} typedef Statement;

struct {
    Statement *statements;
    size_t count;
    size_t allocated;
} typedef Program;

struct {
    Token next_token;

    Lexer lexer;
} typedef Parser;

void parser_init(Parser *parser, Lexer lexer);
Program parser_read_program(Parser *parser);
