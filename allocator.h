#ifndef ALLOCATOR
#define ALLOCATOR

#include "conf.h"

#define memcpyf(dest, src, size) { \
    __typeof__(dest) _dest = dest; \
    __typeof__(src) _src = src; \
    TYPEOF_SIZE _size = (TYPEOF_SIZE) size; \
    while (_size) { \
        *_dest = *_src; \
        _dest++; \
        _src++; \
        _size--; \
    } \
}

void* alloc(TYPEOF_SIZE size);

void* callocm(TYPEOF_SIZE size);

void memcpym(void* dest, const void* src, TYPEOF_SIZE size);

void freem(void* ptr);

void* reallocm(void* ptr, TYPEOF_SIZE size);

#endif
