#ifndef CHUNK_UTILS_H
#define CHUNK_UTILS_H

#include "alloc.h"
#include "compiler_attrs.h"
#include <stddef.h>

static inline t_chunk_header *get_first_chunk(t_zone_header *zone) {
	return (t_chunk_header *)((char *)zone + sizeof(t_zone_header));
}

static inline void *get_ptr_from_chunk(t_chunk_header *chunk) {
	return (void *)((char *)chunk + sizeof(t_chunk_header));
}

static inline t_chunk_header *get_chunk_from_ptr(void *ptr) {
	return (t_chunk_header *)((char *)ptr - sizeof(t_chunk_header));
}

static inline t_chunk_header *next_chunk(t_chunk_header *chunk) {
	return (t_chunk_header *)((char *)chunk + sizeof(t_chunk_header) + chunk->size);
}

t_chunk_header  *find_free_chunk(t_zone_header *zone, size_t size) ATTR_HOT;
void            *carve_chunk(t_zone_header *zone, size_t size) ATTR_HOT;
void            coalesce_forward(t_zone_header *zone, t_chunk_header *chunk);

#endif