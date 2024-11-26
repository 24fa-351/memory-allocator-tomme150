#ifndef HEAP_MEM_H
#define HEAP_MEM_H

#include <stddef.h>

void my_mem_init(size_t total_size);
void my_mem_cleanup();
void* my_malloc(size_t size);
void my_free(void* ptr);
void* my_realloc(void* ptr, size_t size);

#endif // HEAP_MEM_H