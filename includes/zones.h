#ifndef ZONES_H
#define ZONES_H

#include "alloc.h"

t_zone_header   *create_zone(size_t size, t_zone_type type);
void            add_zone(t_zone_header *zone);

t_chunk_header  *get_first_chunk(t_zone_header *zone);
void            *get_ptr_from_chunk(t_chunk_header *chunk);
t_chunk_header  *get_chunk_from_ptr(void *ptr);
t_chunk_header  *find_free_chunk(t_zone_header *zone, size_t size);

#endif
