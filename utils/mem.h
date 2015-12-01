#ifndef MEM_9342_H
#define MEM_9342_H

void *mem_malloc(size_t size);
void *mem_realloc(void *ptr, size_t size);
void mem_free(void *ptr);
#endif
