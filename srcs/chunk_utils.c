#include "alloc.h"

t_chunk_header *get_first_chunk(t_zone_header *zone) {
    if (!zone)
        return NULL;
    return (t_chunk_header *)((char *)zone + sizeof(t_zone_header));
}

void *get_ptr_from_chunk(t_chunk_header *chunk) {
    if (!chunk)
        return NULL;
    return (void *)((char *)chunk + sizeof(t_chunk_header));
}

t_chunk_header *get_chunk_from_ptr(void *ptr) {
    if (!ptr)
        return NULL;
    return (t_chunk_header *)((char *)ptr - sizeof(t_chunk_header));
}

t_chunk_header *find_free_chunk(t_zone_header *zone, size_t size) {
    t_chunk_header *it = get_first_chunk(zone);

    while ( it != NULL && (void *)it < zone->break_ptr) {
        if (it->free == 1 && it->size >= size)
            return it;
        it = (t_chunk_header *)((char *)it + sizeof(t_chunk_header) + it->size);
    }
    return NULL;
}

static inline size_t align_size(size_t size) {
    return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

void *carve_chunk(t_zone_header *zone, size_t size) {
    size_t aligned_size = align_size(size);
    size_t needed = aligned_size + sizeof(t_chunk_header);

    if ( (char *)zone->break_ptr + needed > (char *)zone + zone->zone_size )
        return NULL;

    t_chunk_header *new_chunk = (t_chunk_header *)zone->break_ptr;

    new_chunk->next = NULL;
    new_chunk->size = aligned_size;
    new_chunk->free = 0;

    zone->break_ptr = (char *)zone->break_ptr + needed;

    return get_ptr_from_chunk(new_chunk);
}
