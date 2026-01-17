#ifndef SIZE_UTILS_H
#define SIZE_UTILS_H

#include <stddef.h>
#include <unistd.h>

// (page size / ratio) * min_alloc_count = size of the chunk
#define TINY_RATIO 32
#define SMALL_RATIO 4
#define MIN_ALLOC_COUNT 100

size_t get_system_page_size();
size_t get_tiny_max(void);
size_t get_small_max(void);
size_t get_zone_size(size_t max_type_size);

#endif
