#include "lexer.h"
#include <stdlib.h>
#include "parser.h"

void parser_advance(Parser *parser) {
    parser->next_token = lexer_next_token(&parser->lexer);
}

void parser_init(Parser *parser, Lexer lexer) {
    parser->lexer = lexer;
    parser_advance(parser);
}

Operand parser_read_operand(Parser *parser) {
    Operand operand;
    switch (parser->next_token.type) {
        case REGISTER:
            operand = (Operand) { .type = OPERAND_REGISTER, .data.reg.reg = parser->next_token.data.reg.reg };
            parser_advance(parser);
            break;
        case VALUE:
            operand = (Operand) { .type = OPERAND_VALUE, .data.value.value = parser->next_token.data.value.value };
            parser_advance(parser);
            break;
        default:
            return (Operand) { .type = OPERAND_INVALID, .data.error = parser->next_token };
    }
    return operand;
}

Statement parser_read_statement(Parser *parser) {
    Statement statement;
    if (parser->next_token.type == MNEMONIC) {
        statement.type = STATEMENT_INSTRUCTION;
        statement.data.instruction.mnemonic = parser->next_token.data.mnemonic.mnemonic;
        statement.data.instruction.operand_count = 0;
        parser_advance(parser);
        Operand operand; 
        while ((operand = parser_read_operand(parser)).type != OPERAND_INVALID) {
            statement.data.instruction.operands[statement.data.instruction.operand_count] = operand;
            statement.data.instruction.operand_count++;
            if (parser->next_token.type != COMMA) {
                break;
            }
            parser_advance(parser);
            if (statement.data.instruction.operand_count == 3) {
                break;
            }
        }
        return statement;
    }
    if (parser->next_token.type == END) {
        statement.type = STATEMENT_END;
        return statement;
    }
    statement.type = STATEMENT_INVALID;
    statement.data.error.token = parser->next_token;
    return statement;
}

Program parser_read_program(Parser *parser) {
    Program program = { .count = 0, .statements = malloc(sizeof(Statement) * 8), .allocated = 8 };
    Statement statement;
    while ((statement = parser_read_statement(parser)).type != STATEMENT_END) {
        if (program.count == program.allocated) {
            program.allocated *= 2;
            program.statements = realloc(program.statements, sizeof(Statement) * program.allocated);
        }
        program.statements[program.count] = statement;
        program.count++;
        if (statement.type == STATEMENT_INVALID) {
            parser_advance(parser);
        }
    }
    return program;
}
