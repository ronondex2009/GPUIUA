#define CL_TARGET_OPENCL_VERSION 300

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#include "CL\opencl.h"

// INTERFACE //

// INTERNAL //

void checkError(cl_int err, const char* str) {
    if (err != CL_SUCCESS) {
        errno = err;
        printf("\n");
        printf(str);
        printf("\nerr:%d\n", err);
        assert(err == CL_SUCCESS);
    }
}



int main(int arg, char* args[]) {
    printf("Hello world!");

    const unsigned int len = pow(2, 20);
    cl_int err;
    cl_platform_id platforms[64];
    unsigned int num_platforms;

    err = clGetPlatformIDs(64, platforms, &num_platforms);
    checkError(err, "Getting platforms");

    cl_device_id device;

    for(int i = 0; i < num_platforms; i++) {
        cl_device_id devices[64];
        unsigned int num_devices;
        cl_int result = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 64, devices, &num_devices);
        if (result == CL_SUCCESS) {
            for (int j = 0; j < num_devices; j++) {
                char vendor_name[256];
                unsigned int vendor_name_len;

                cl_int result = clGetDeviceInfo(devices[j], CL_DEVICE_VENDOR, 256, vendor_name, &vendor_name_len);
                
                if (result == CL_SUCCESS) {
                    device = devices[j];
                    break;
                }

                printf(vendor_name);
            }
        }
    }

    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    checkError(err, "Creating context");

    cl_queue_properties queue_properties[] = {0};
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, queue_properties, &err);
    checkError(err, "Creating queue");

    const char* program_source = 
    "__kernel void vector_sum(__constant float* a, __constant float* b, __global float* c) {int i = get_global_id(0); c[i] = a[i] + b[i];}";
    size_t length = 0;
    cl_program program = clCreateProgramWithSource(context, 1, &program_source, &length, &err);
    checkError(err, "Building program");

    cl_int program_build_result = clBuildProgram(program, 1, &device, "", NULL, NULL);
    if (err != CL_SUCCESS) {
        char log[256];
        size_t logLength;
        cl_int program_build_info_result = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 256, log, &err);
        checkError(err, "Getting platform build error log");
        printf(log);
    }

    cl_kernel kernel = clCreateKernel(program, "vector_sum", &err);
    checkError(err, "Making kernel");    

    printf("allocating\n");
    float* veca_data = (float*) malloc(sizeof(float) * len);
    float* vecb_data = (float*) malloc(sizeof(float) * len);
    float* vecc_data = (float*) malloc(sizeof(float) * len);
    printf("setting\n");

    for (unsigned int i = 0; i < len; i++) {
        veca_data[i] = rand();
        vecb_data[i] = rand();
    }
    printf("buffering!!");

    cl_int veca_result, vecb_result, vecc_result;
    cl_mem veca = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, len * sizeof(float), veca_data, &veca_result);
    checkError(err, "Creating memory buffers");
    cl_mem vecb = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, len * sizeof(float), vecb_data, &vecb_result);
    checkError(err, "Creating memory buffers");
    cl_mem vecc = clCreateBuffer(context, CL_MEM_WRITE_ONLY, len * sizeof(float), NULL, &vecc_result);
    checkError(err, "Creating memory buffers");

    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &veca);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &vecb);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &vecc);
    checkError(err, "Setting args");

    size_t global_work_size = len;
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
    checkError(err, "Enqueueing kernel jobs");    

    err = clEnqueueReadBuffer(queue, vecc, CL_TRUE, 0, len * sizeof(float), vecc_data, 0, NULL, NULL);
    checkError(err, "Reading buffers");

    printf("finishing");

    err = clFinish(queue);
    checkError(err, "Reading buffers");

    for (int i = 0; i < len; i++) {
        char* outstr;
        outstr = itoa(vecc_data[i], outstr, 10);
        // printf(outstr);
        // printf("\n");
    }

    printf("END");
    return 1;
}