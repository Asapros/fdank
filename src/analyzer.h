#pragma once

#include "parser.h"
struct {
    bool valid;
    const char *description;
} typedef SemanticAnalysis;

SemanticAnalysis validate_statement(Statement statement);
