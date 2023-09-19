#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "conf.h"
#include "allocator.h"

typedef struct block BLOCK_ATTRIBUTES {
    TYPEOF_SIZE size;
    bool used;
    struct block* next;
    intptr_t data[1]; // dont change placement, the struct always needs to be behind the data
} block;

#if IS_ALIGNED == true
    static TYPEOF_SIZE align(TYPEOF_SIZE num) {
        return (num + sizeof(intptr_t) - 1) & ~(sizeof(intptr_t) - 1);
    }
#endif

static TYPEOF_SIZE alloc_size(TYPEOF_SIZE size) {
    return sizeof(block)+size-sizeof(intptr_t);
}

static block* req_mem(TYPEOF_SIZE size) {
    block* blck = (block*) sbrk(0);

    if (sbrk(alloc_size(size)) == (void*)-1) { // (void*)-1 is an error value that says that the memory cant be allocated
        return NULL;
    }
    return blck;
}

static block* header = NULL;

void* alloc(TYPEOF_SIZE size) {
    if (size == 0) {
        return NULL;
    }

    #if IS_ALIGNED == true
        size = align(size);
    #endif
    if (header == NULL) {
        block* b = req_mem(size);
        if (b == NULL) {
            return NULL;
        }
        b->size = size;
        b->used = true;
        b->next = NULL;
        header = b;
        return b->data;
    }

    block* curr = header;

    while (curr->next != NULL) { // todo
        if (curr->size == size && curr->used == false) {
            curr->used = true;
            return curr->data;
        }
        curr = curr->next;
    }
    block* b = req_mem(size);
    if (b == NULL) {
        return NULL;
    }
    b->size = size;
    b->used = true;
    b->next = NULL;
    curr->next = b;
    return b->data;
}

void memcpym(void* dest, const void* src, TYPEOF_SIZE size) {
	char* d = dest;
    const char* s = src;
    while (size--) {
        *d = *s;
        d++;
        s++;
    }
}

void freem(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    block* tofree = ptr-sizeof(block)+sizeof(intptr_t);
    if (tofree->size+ptr == sbrk(0)) {
        if (header == tofree) {
            header = NULL;
        }
        else {
            block* curr = header;
            while (curr->next != tofree) { curr = curr->next; }
            if (curr->next->next == NULL) {
                curr->next = NULL;
            }
            else {
                curr->next = curr->next->next;
            }
        }
        sbrk(-(int)alloc_size(tofree->size));
        return;
    }
    tofree->used = false;
}

void* reallocm(void* ptr, TYPEOF_SIZE size) {
    if (ptr == NULL || size == 0) {
        return NULL;
    }
    
    block* tofree = ptr-sizeof(block)+sizeof(intptr_t);
    TYPEOF_SIZE amtocpy = size;
    if (tofree->size < size) {
        amtocpy = tofree->size;
    }

    void* temp = ptr;
    ptr = alloc(size);
    memcpym(ptr, temp, amtocpy);
    freem(temp);

    return ptr;
}

/*int main() {
    int* a = alloc(sizeof(int));
    *a = 69;
    int* b = alloc(sizeof(int));
    *b = 420;
    int c = 1;
    memcpyf(a, b, c);
    printf("%d\n", *a);
    freem(b);
    freem(a);
}*/
