#include "heap_mem.h"
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

#define ALIGNMENT 8 // Memory alignment to 8 bytes
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

typedef struct Block {
    size_t size;         // Size of the block
    bool is_free;        // Block status (free/allocated)
    struct Block* next;  // Next block in the list
} Block;

static void* heap_start = NULL; // Start of the memory block
static Block* free_list = NULL; // Linked list of free blocks
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // For thread safety

// Helper function: Request memory from the system
void* request_memory(size_t size) {
    void* ptr = sbrk(0);
    if (sbrk(size) == (void*)-1) {
        return NULL;
    }
    return ptr;
}

// Helper function: Split a block if it's too large
void split_block(Block* block, size_t size) {
    if (block->size >= size + sizeof(Block) + ALIGNMENT) {
        Block* new_block = (Block*)((char*)block + sizeof(Block) + size);
        new_block->size = block->size - size - sizeof(Block);
        new_block->is_free = true;
        new_block->next = block->next;
        block->size = size;
        block->next = new_block;
    }
}

// Helper function: Merge adjacent free blocks
void merge_blocks() {
    Block* current = free_list;
    while (current && current->next) {
        if (current->is_free && current->next->is_free) {
            current->size += sizeof(Block) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

// Initialization function
void my_mem_init(size_t total_size) {
    pthread_mutex_lock(&lock);
    heap_start = request_memory(total_size);
    if (!heap_start) {
        fprintf(stderr, "Failed to initialize memory.\n");
        pthread_mutex_unlock(&lock);
        return;
    }

    free_list = (Block*)heap_start;
    free_list->size = total_size - sizeof(Block);
    free_list->is_free = true;
    free_list->next = NULL;
    pthread_mutex_unlock(&lock);
}

// Cleanup function (for testing)
void my_mem_cleanup() {
    pthread_mutex_lock(&lock);
    brk(heap_start);
    heap_start = NULL;
    free_list = NULL;
    pthread_mutex_unlock(&lock);
}

// malloc implementation
void* my_malloc(size_t size) {
    pthread_mutex_lock(&lock);
    size = ALIGN(size);
    Block* current = free_list;

    while (current) {
        if (current->is_free && current->size >= size) {
            current->is_free = false;
            split_block(current, size);
            pthread_mutex_unlock(&lock);
            return (void*)((char*)current + sizeof(Block));
        }
        current = current->next;
    }

    pthread_mutex_unlock(&lock);
    return NULL; // No suitable block found
}

// free implementation
void my_free(void* ptr) {
    if (!ptr) return;

    pthread_mutex_lock(&lock);
    Block* block = (Block*)((char*)ptr - sizeof(Block));
    block->is_free = true;
    merge_blocks();
    pthread_mutex_unlock(&lock);
}

// realloc implementation
void* my_realloc(void* ptr, size_t size) {
    if (!ptr) return my_malloc(size);

    Block* block = (Block*)((char*)ptr - sizeof(Block));
    if (block->size >= size) {
        return ptr; // Already large enough
    }

    void* new_ptr = my_malloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, block->size);
        my_free(ptr);
    }
    return new_ptr;
}
