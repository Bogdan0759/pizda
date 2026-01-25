#include <lib/com1.h>
#include <mlibc/memory.h>

extern char kernel_end;
#define HEAP_SIZE (128 * 1024)
static char *heap_start = 0;
static char *heap_end = 0;

typedef struct header {
  struct header *next;
  unsigned long size;
  long is_free;
  long reserved;
} header_t;

static header_t *free_list = 0;

static unsigned long align16(unsigned long size) {
  return (size + 15) & ~15UL;
}

static void split_block(header_t *current, unsigned long size) {
  if (current->size >= size + sizeof(header_t) + 16) {
    header_t *new_block =
        (header_t *)((char *)current + sizeof(header_t) + size);
    new_block->size = current->size - size - sizeof(header_t);
    new_block->next = current->next;
    new_block->is_free = 1;

    current->size = size;
    current->next = new_block;
  }
}

static void coalesce_free_list() {
  header_t *current = free_list;
  while (current && current->next) {
    if (current->is_free && current->next->is_free) {
      current->size += current->next->size + sizeof(header_t);
      current->next = current->next->next;
      continue;
    }
    current = current->next;
  }
}

void init_heap() {
  heap_start = (char *)&kernel_end;
  heap_start = (char *)align16((unsigned long)heap_start);
  heap_end = heap_start + HEAP_SIZE;

  free_list = (header_t *)heap_start;
  free_list->size = HEAP_SIZE - sizeof(header_t);
  free_list->next = 0;
  free_list->is_free = 1;

  com1_printf("Heap initialized at %p (header size: %d)\n", heap_start,
              (int)sizeof(header_t));
  com1_printf("Free block size: %d\n", (int)free_list->size);
}

void *kmalloc(unsigned long size) {
  if (!free_list)
    init_heap();

  if (size == 0)
    return 0;

  size = align16(size);

  header_t *current = free_list;
  header_t *best = 0;
  unsigned long best_size = ~0UL;

  while (current) {
    if (current->is_free && current->size >= size) {
      if (current->size < best_size) {
        best = current;
        best_size = current->size;
        if (current->size == size)
          break;
      }
    }
    current = current->next;
  }

  if (!best) {
    com1_printf("KMALLOC FAILED! Request size: %d\n", (int)size);
    if (free_list) {
      com1_printf("First block: free=%d, size=%d\n", (int)free_list->is_free,
                  (int)free_list->size);
    }
    return 0;
  }

  split_block(best, size);
  best->is_free = 0;
  return (void *)((char *)best + sizeof(header_t));
}

void kfree(void *ptr) {
  if (!ptr)
    return;
  if (!heap_start || !heap_end)
    return;
  if ((char *)ptr < heap_start + sizeof(header_t) || (char *)ptr >= heap_end)
    return;

  header_t *header = (header_t *)((char *)ptr - sizeof(header_t));
  if (header->is_free)
    return;

  header->is_free = 1;
  coalesce_free_list();
}

void *kcalloc(unsigned long nmemb, unsigned long size) {
  if (nmemb == 0 || size == 0)
    return 0;
  if (size != 0 && nmemb > (~0UL) / size)
    return 0;

  unsigned long total = nmemb * size;
  void *ptr = kmalloc(total);
  if (ptr) {
    unsigned char *p = (unsigned char *)ptr;
    for (unsigned long i = 0; i < total; i++)
      p[i] = 0;
  }
  return ptr;
}
