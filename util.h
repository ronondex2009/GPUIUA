#ifndef UTIL_H
#define UTIL_H

// stack of void* pointers rather than type for compatability
typedef struct PointerStack{
    void** arr;
    int top;
} Stack;

#endif
