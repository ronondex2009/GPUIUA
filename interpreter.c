#include "interpreter.h"

void checkError(cl_int err, const char* str) {
    if (err != CL_SUCCESS) {
        errno = err;
        printf("\n");
        printf(str);
        printf("\nerr:%d\n", err);
    }
    assert(err == CL_SUCCESS);
}

// find first available OpenCL platform
cl_platform_id get_default_platform() {
    cl_int err;
    cl_platform_id first_platform;
    cl_uint num_platforms;
    err = clGetPlatformIDs(1, &first_platform, &num_platforms);
    checkError(err, "failed to get the first available platform\n");
    return first_platform;
}

// find first available GPU device
cl_device_id get_default_gpu(cl_platform_id platform_id) {
    cl_int err;
    cl_device_id first_device;
    cl_uint num_devices;
    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &first_device, &num_devices);
    checkError(err, "failed to get first available GPU device from platform\n");
    return first_device;
}

// the one and only function
// interprets GPUIUA code
double** gpuiua_run(char* code, double** flat_arrays_input, unsigned int num_arrays_input, unsigned int num_arrays_output, unsigned int arrays_length) {
    Node* root_node_parse_tree = parse(code); // root node of the parse tree
    if (!validate_tree(root_node_parse_tree)) {
        // huh.. something went wrong.
        printf("GPUIUA Interpreter couldnt validate the parse tree! Dumping tree!\n");
        printf("%s\n", tree_structure_to_string(*root_node_parse_tree));
        return NULL;
    }

    cl_platform_id current_platform = get_default_platform();
    cl_device_id current_device = get_default_gpu(current_platform);
    cl_int err;

    // create context
    InterpreterContext* interpreter_context = (InterpreterContext*) calloc(sizeof(InterpreterContext), 1);
    interpreter_context->selected_platform  = current_platform;
    interpreter_context->selected_devices   = &current_device;
    interpreter_context->num_devices        = 1;
    interpreter_context->clcontext          = clCreateContext(NULL, 1, &current_device, NULL, NULL, &err);
    checkError(err, "failed to create cl_context\n");

    // create buffer stack

}