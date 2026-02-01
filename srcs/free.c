#include "alloc.h"
#include "zones.h"
#include "chunk_utils.h"

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
   t_zone_header *zone;

   zone = find_zone_for_ptr(ptr, ZONE_TINY);
   if (zone) {
       chunk->free = 1;
       return;
   }

   zone = find_zone_for_ptr(ptr, ZONE_SMALL);
   if (zone) {
       chunk->free = 1;
       return;
   }

   zone = find_zone_for_ptr(ptr, ZONE_LARGE);
   if (zone) {
       remove_zone(zone);
       munmap((void *)zone, zone->zone_size);
       return;
   }

   ft_putendl_fd("Error: invalid free", 1);
}
