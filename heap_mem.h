#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stddef.h>

// Memory management functions
void* my_malloc(size_t size);
void my_free(void* ptr);
void* my_realloc(void* ptr, size_t size);

// Initialization function
void my_mem_init(size_t total_size);

// Cleanup function (for testing)
void my_mem_cleanup();

#endif // MY_MALLOC_H

