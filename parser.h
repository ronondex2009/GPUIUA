/*
    parser.h and parser.c contains code for parsing gpuiua code into a parse tree
*/

#ifndef GPUIUA_H
#define GPUIUA_H

#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "util.h"
#include "nodetree.h"

#ifdef DEBUG
#define dbputs(str) (puts(str))
#else
#define dbputs(str) ()
#endif

typedef unsigned int uint;

typedef enum {NO_ERROR, EXPECT_ERR, INVALID_UTF} error_t;
typedef enum {false, true} bool;

Node* parse(char* input);

#endif