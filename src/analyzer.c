#include "analyzer.h"
#include "lexer.h"
#include "parser.h"
#include <stdbool.h>

struct {
    Mnemonic mnemonic;
    bool counts[4];
} typedef ValidOperandCountLookupPair;

static ValidOperandCountLookupPair ALLOWED_OPERAND_COUNTS[] = {
    {MOV,     {0,0,1,0}},
    {SYSCALL, {1,0,0,0}},
};

SemanticAnalysis validate_instruction(StatementInstructionData instruction) {
    for (size_t i = 0; i < sizeof(ALLOWED_OPERAND_COUNTS) / sizeof(ValidOperandCountLookupPair); i++) {
        if (instruction.mnemonic != ALLOWED_OPERAND_COUNTS[i].mnemonic) continue;
        if (!ALLOWED_OPERAND_COUNTS[i].counts[instruction.operand_count]) {
            return (SemanticAnalysis) { .valid = false, .description = "Invalid number of operands" };
        }
    }
    switch (instruction.mnemonic) {
        case MOV:
            if (instruction.operands[0].type != OPERAND_REGISTER) {
                return (SemanticAnalysis) { .valid = false, .description = "Invalid operand: mov requires a register as the first operand" };
            }
            if (instruction.operands[1].type != OPERAND_REGISTER && instruction.operands[1].type != OPERAND_VALUE) {
                return (SemanticAnalysis) { .valid = false, .description = "Invalid operand: mov requires a register or a value as the second operand" };
            }
            return (SemanticAnalysis) { .valid = true };
        case SYSCALL:
            return (SemanticAnalysis) { .valid = true };
        case INVALID_MNEMONIC:
            return (SemanticAnalysis) { .valid = false, .description = "Invalid mnemonic" };
        }
    return (SemanticAnalysis) { .valid = false, .description = "Unreachable branch" };
}


SemanticAnalysis validate_statement(Statement statement) {
    switch (statement.type) {
        case STATEMENT_INSTRUCTION:
            return validate_instruction(statement.data.instruction);
        case STATEMENT_END:
            return (SemanticAnalysis) { .valid = true };
        case STATEMENT_INVALID:
            return (SemanticAnalysis) { .valid = false, .description = "Invalid instruction: unexpected token" };
        }
    return (SemanticAnalysis) { .valid = false, .description = "Unreachable branch" };
}
