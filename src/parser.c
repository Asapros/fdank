#include "lexer.h"
#include <stdio.h>
#include "parser.h"

void parser_advance(Parser *parser) {
    parser->next_token = lexer_next_token(&parser->lexer);
}

void parser_init(Parser *parser, Lexer lexer) {
    parser->lexer = lexer;
    parser_advance(parser);
}

void parser_read_mov(Parser *parser) {
    if (parser->next_token.type != OP_MOV) {
        return;
    }
    parser_advance(parser);
    if (!is_register(parser->next_token.type)) {
        return;
    }
    parser_advance(parser);
    if (parser->next_token.type != COMMA) {
        return;
    }
    parser_advance(parser);
    if (is_register(parser->next_token.type)) {
        parser_advance(parser);
        printf("register register mov\n");
        return;
    }
    if (parser->next_token.type == INTEGER) {
        parser_advance(parser);
        printf("register integer mov\n");
        return;
    }
}
void parser_read_syscall(Parser *parser) {
    parser_advance(parser);
    printf("syscall\n");
    return;
}

void parser_next_instruction(Parser *parser) {
    if (parser->next_token.type == OP_MOV) {
        parser_read_mov(parser);
        return;
    }
    if (parser->next_token.type == OP_SYSCALL) {
        parser_read_syscall(parser);
        return;
    }
}
