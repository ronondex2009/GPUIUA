/*
    interpreter takes parse trees and runs it on the GPU with array inputs
*/

#ifndef PARSE_TREE_INTERPRETER_H
#define PARSE_TREE_INTERPRETER_H

#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "CL\cl.h"
#include "nodetree.h"
#include "parser.h"

typedef struct {
    cl_context clcontext;
    cl_platform_id selected_platform;
    cl_device_id* selected_devices;
    unsigned int num_devices;
    Stack* buffer_stack;
} InterpreterContext; // placeholder right now

#endif