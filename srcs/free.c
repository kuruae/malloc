#include "alloc.h"
#include "size_utils.h"
#include "zones.h"
#include <stdio.h>
#include <sys/mman.h>

t_zone_header *find_zone_for_ptr(void *ptr, t_zone_type type) {
    t_zone_header *zone;
    if (type == ZONE_TINY)
        zone = g_zones.tiny;
    else if (type == ZONE_SMALL)
        zone = g_zones.small;
    else
        zone = g_zones.large;
    
    while (zone) {
         if (ptr >= (void *)zone && ptr < (void *)zone + zone->zone_size)
             return zone;
        zone = zone->next;
    }
    return NULL;
}

void free(void *ptr) {
    if (!ptr)
        return;
    
   t_chunk_header *chunk = get_chunk_from_ptr(ptr);
   if (chunk->size <= get_tiny_max())
       (void)ptr;
   if (chunk->size <= get_small_max())
       (void)ptr;
   else {
       t_zone_header *zone = find_zone_for_ptr(ptr, ZONE_LARGE);
       remove_zone(zone);
       munmap((void *)zone, zone->zone_size);
   }
}