#pragma once

#include "lexer.h"

#define PARSER_BUFFER_SIZE 8

struct {
    Token next_token;

    Lexer lexer;
} typedef Parser;

void parser_init(Parser *parser, Lexer lexer);
void parser_next_instruction(Parser *parser);
