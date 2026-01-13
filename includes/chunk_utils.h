#ifndef CHUNK_UTILS_H
#define CHUNK_UTILS_H

#include <stddef.h>
#include <unistd.h>

#define TINY_RATIO 32
#define SMALL_MAX 1024
#define MIN_ALLOC_COUNT 100

size_t get_tiny_max(void);
size_t get_small_max(void);
size_t get_zone_size(size_t max_type_size);

#endif
