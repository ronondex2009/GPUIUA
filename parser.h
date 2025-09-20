#ifndef GPUIUA_H
#define GPUIUA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "util.h"
#include "nodetree.h"

typedef unsigned int uint;

typedef enum {NO_ERROR, EXPECT_ERR, INVALID_UTF} error_t;
typedef enum {false, true} bool;

// prototypes
// root       := expression+                        <-- PARSE ENTERS HERE
// expression := modifier <expression> | function
// function   := '(' expression+ <')'> | Pervasive | literal
// modifier   := monmodifiers (subscript)?
Node* _parse_root(char** tokens, int* cursor, int num_tokens);
bool _parse_expression(char** tokens, int* cursor, int num_tokens, Node* parent_node);
bool _parse_function(char** tokens, int* cursor, int num_tokens, Node* parent_node);
bool _parse_modifier(char** tokens, int* cursor, int num_tokens, Node* parent_node);

#endif