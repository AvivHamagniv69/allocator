#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct block {
    size_t size;
    bool used;
    struct block* next;
    intptr_t data[1]; // dont change placement, the struct always needs to be behind the data
} block;

static size_t align(size_t num) {
    return (num + sizeof(intptr_t) - 1) & ~(sizeof(intptr_t) - 1);
}

static size_t alloc_size(size_t size) {
    return sizeof(block)+size-sizeof(intptr_t);
}

static block* req_mem(size_t size) {
    block* blck = (block*) sbrk(0);

    if (sbrk(alloc_size(size)) == (void*)-1) { // (void*)-1 is an error value that says that the memory cant be allocated
        return NULL;
    }
    return blck;
}

static block* header = NULL;

void* alloc(size_t size) {
    size = align(size);
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

int main() {
    int* a = alloc(sizeof(int));
    *a = 69;
    int* b = alloc(sizeof(int));
    *b = 420;
    printf("%d\n", *a);
    freem(b);
    freem(a);
}
