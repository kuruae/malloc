#ifndef CHUNK_UTILS_H
#define CHUNK_UTILS_H

#include "alloc.h"
#include <stddef.h>

t_chunk_header  *get_first_chunk(t_zone_header *zone);
void            *get_ptr_from_chunk(t_chunk_header *chunk);
t_chunk_header  *get_chunk_from_ptr(void *ptr);
t_chunk_header  *find_free_chunk(t_zone_header *zone, size_t size);
void            *carve_chunk(t_zone_header *zone, size_t size);

#endif