#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_malloc_free() {
    printf("Running test_malloc_free...\n");
    my_mem_init(1024);
    void* ptr = my_malloc(100);
    if (ptr) {
        printf("Allocation successful.\n");
    } else {
        printf("Allocation failed.\n");
    }
    my_free(ptr);
    my_mem_cleanup();
    printf("test_malloc_free passed.\n");
}

void test_realloc() {
    printf("Running test_realloc...\n");
    my_mem_init(1024);
    void* ptr = my_malloc(100);
    if (ptr) {
        printf("Allocation successful.\n");
    } else {
        printf("Allocation failed.\n");
    }
    ptr = my_realloc(ptr, 200);
    if (ptr) {
        printf("Reallocation successful.\n");
    } else {
        printf("Reallocation failed.\n");
    }
    my_free(ptr);
    my_mem_cleanup();
    printf("test_realloc passed.\n");
}

int main(int argc, char* argv[]) {
    test_malloc_free();
    test_realloc();
    return 0;
}