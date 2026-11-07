#include "generator.h"
#include "lexer.h"
#include "parser.h"
const RegisterNumberLookupPair REGISTER_NUMBERS[] = {
    { EAX, 0 },
    // { ECX, 1 },
    // { EDX, 2 },
    // { EBX, 3 },
    // { ESP, 4 },
    // { EBP, 5 },
    // { ESI, 6 },
    { EDI, 7 }
};


AssembledBytecode generate_instruction_bytecode(StatementInstructionData instruction) {
    switch (instruction.mnemonic) {
        case SYSCALL:
            return (AssembledBytecode) { .size = 2, .binary = { 0x0F, 0x05 } };
        case MOV: {
            unsigned char register_number;
            for (int i = 0; i < sizeof(REGISTER_NUMBERS) / sizeof(RegisterNumberLookupPair); i++) {
                if (REGISTER_NUMBERS[i].reg == instruction.operands[0].data.reg.reg) register_number = REGISTER_NUMBERS[i].number;
            }
            unsigned char opcode = 0xB8 | register_number;
            AssembledBytecode bytecode = { .size = 5 };
            bytecode.binary[0] = opcode;
            bytecode.binary[1] =  instruction.operands[1].data.value.value & 0xFF;
            bytecode.binary[2] = (instruction.operands[1].data.value.value >>  8) & 0xFF;
            bytecode.binary[3] = (instruction.operands[1].data.value.value >> 16) & 0xFF;
            bytecode.binary[4] = (instruction.operands[1].data.value.value >> 24) & 0xFF;
            return bytecode;
        }
        case INVALID_MNEMONIC:
            break;
    }
    return (AssembledBytecode) { .size = 0 };
}


AssembledBytecode generate_statement_bytecode(Statement statement) {
    switch (statement.type) {
        case STATEMENT_INSTRUCTION:
            return generate_instruction_bytecode(statement.data.instruction);
        case STATEMENT_END:
        case STATEMENT_INVALID:
            break;
    }
    return (AssembledBytecode) { .size = 0 };
}
