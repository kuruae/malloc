#include "alloc.h"
#include <stddef.h>

t_zones g_zones = {NULL, NULL, NULL};

// Creates mmap'd zone with specified size and type
t_zone_header *create_zone(size_t size, t_zone_type type) {
    void *ptr = mmap(
        NULL,
        size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    if (ptr == MAP_FAILED)
        return NULL;

    t_zone_header *zone = (t_zone_header *)ptr;
    zone->next = NULL;
    zone->zone_size = size;
    zone->type = type;
    zone->break_ptr = (char *)zone + sizeof(t_zone_header);

    return zone;
}

// Adds zone to head of appropriate linked list (tiny/small/large)
void add_zone(t_zone_header *zone) {
    if (!zone)
        return;

    t_zone_header **list = NULL;

    if (zone->type == ZONE_TINY)
        list = &g_zones.tiny;
    else if (zone->type == ZONE_SMALL)
        list = &g_zones.small;
    else
        list = &g_zones.large;

    zone->next = *list;
    *list = zone;
}

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

// Reverse of get_ptr_from_chunk - used in free()
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

void *carve_chunk(t_zone_header *zone, size_t size) {
    size_t needed = size + sizeof(t_chunk_header);
    
    if ( (char *)zone->break_ptr + needed > (char *)zone + zone->zone_size )
        return NULL;
    
    t_chunk_header *new_chunk = (t_chunk_header *)zone->break_ptr;
    
    new_chunk->next = NULL;
    new_chunk->size = size;
    new_chunk->free = 0;
    
    zone->break_ptr = (char *)zone->break_ptr + needed;
    
    return get_ptr_from_chunk(new_chunk);
}