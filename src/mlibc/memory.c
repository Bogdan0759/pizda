#include <lib/com1.h>
#include <mlibc/memory.h>

extern char kernel_end; 

#define HEAP_SIZE (128 * 1024) 
static char *heap_start = 0;
static char *heap_end = 0;

typedef struct header {
  struct header *next;
  unsigned long size;
  int is_free;
} header_t;

static header_t *free_list = 0;

void init_heap() {
  heap_start = (char *)&kernel_end;
  heap_start = (char *)(((unsigned long)heap_start + 15) & ~15);
  heap_end = heap_start + HEAP_SIZE;

  free_list = (header_t *)heap_start;
  free_list->size = HEAP_SIZE - sizeof(header_t);
  free_list->next = 0;
  free_list->is_free = 1;

  com1_printf("Heap initialized at %p, size %d bytes\n", heap_start, HEAP_SIZE);
}

void *kmalloc(unsigned long size) {
  if (!free_list)
    init_heap();

  size = (size + 15) & ~15;

  header_t *current = free_list;
  while (current) {
    if (current->is_free && current->size >= size) {
      if (current->size > size + sizeof(header_t) + 16) {
        header_t *new_block =
            (header_t *)((char *)current + sizeof(header_t) + size);
        new_block->size = current->size - size - sizeof(header_t);
        new_block->next = current->next;
        new_block->is_free = 1;

        current->size = size;
        current->next = new_block;
      }
      current->is_free = 0;
      return (void *)((char *)current + sizeof(header_t));
    }
    current = current->next;
  }

  com1_printf("KMALLOC FAILED for size %d!\n", (int)size);
  return 0;
}

void kfree(void *ptr) {
  if (!ptr)
    return;

  header_t *header = (header_t *)((char *)ptr - sizeof(header_t));
  header->is_free = 1;

  header_t *current = free_list;
  while (current) {
    if (current->is_free && current->next && current->next->is_free) {
      current->size += current->next->size + sizeof(header_t);
      current->next = current->next->next;
      continue;
    }
    current = current->next;
  }
}

void *kcalloc(unsigned long nmemb, unsigned long size) {
  unsigned long total = nmemb * size;
  void *ptr = kmalloc(total);
  if (ptr) {
    char *p = (char *)ptr;
    for (unsigned long i = 0; i < total; i++)
      p[i] = 0;
  }
  return ptr;
}
