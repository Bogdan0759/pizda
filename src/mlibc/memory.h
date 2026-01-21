#ifndef MEMORY_H
#define MEMORY_H

#include <mlibc/mlibc.h>

void *kmalloc(unsigned long size);
void kfree(void *ptr);
void *kcalloc(unsigned long nmemb, unsigned long size);
void init_heap();

#endif
