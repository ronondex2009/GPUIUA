/*
    util.h has code for stuff like:
        - the stack
    .. and thats it right now
*/

#ifndef UTIL_H
#define UTIL_H

// stack of void* pointers rather than type for compatability
typedef struct PointerStack{
    void** arr;
    int top;
} Stack;

#endif
