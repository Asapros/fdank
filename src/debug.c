#include "debug.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>

const char *stringify_register(Register reg) {
    for (int i = 0; i < sizeof(REGISTER_NAMES) / sizeof(RegisterLookupPair); i++) {
        if (REGISTER_NAMES[i].reg == reg) return REGISTER_NAMES[i].name;
    }
    return "REGISTER_LOOKUP_ERROR";
}

const char *stringify_mnemonic(Mnemonic mnemonic) {
    for (int i = 0; i < sizeof(MNEMONIC_NAMES) / sizeof(MnemonicLookupPair); i++) {
        if (MNEMONIC_NAMES[i].mnemonic == mnemonic) return MNEMONIC_NAMES[i].name;
    }
    return "MNEMONIC_LOOKUP_ERROR";
}

void print_token(Token token) {
    switch (token.type) {
        case IDENTIFIER:
            printf("IDENTIFIER(%s)", token.data.identifier.name);
            return;
        case VALUE:
            printf("VALUE(%lld)", token.data.value.value);
            return;
        case MNEMONIC:
            printf("MNEMONIC(%s)", stringify_mnemonic(token.data.mnemonic.mnemonic));
            return;
        case REGISTER:
            printf("REGISTER(%s)", stringify_register(token.data.reg.reg));
            return;
        case COMMA:
            printf("COMMA");
            return;
        case END:
            printf("END");
            return;
        case ERROR:
            printf("ERROR(%u)", token.data.error.error);
            return;
    }
}

void print_operand(Operand operand) {
    switch (operand.type) {
        case OPERAND_REGISTER:
            printf("OPERAND_REGISTER(%s)", stringify_register(operand.data.reg.reg));
            return;
        case OPERAND_VALUE:
            printf("OPERAND_VALUE(%lld)", operand.data.value.value);
            return;
        case OPERAND_INVALID:
            printf("OPERAND_INVALID(");
            print_token(operand.data.error.token);
            printf(")");
            return;
    }
}

void print_statement(Statement statement) {
    switch (statement.type) {
        case STATEMENT_INSTRUCTION:
            printf("STATEMENT_INSTRUCTION(\n\tMNEMONIC(%s)\n", stringify_mnemonic(statement.data.instruction.mnemonic));
            for (int i = 0; i < statement.data.instruction.operand_count; i++) {
                printf("\t");
                print_operand(statement.data.instruction.operands[i]);
                printf("\n");
            }
            printf(")");
            return;
        case STATEMENT_END:
            printf("STATEMENT_END");
            return;
        case STATEMENT_INVALID:
            printf("STATEMENT_INVALID(");
            print_token(statement.data.error.token);
            printf(")");
            return;
        }
}
