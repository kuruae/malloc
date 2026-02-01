#include "alloc.h"
#include "zones.h"
#include "chunk_utils.h"

t_zone_header *find_zone_for_ptr(const void *ptr, t_zone_type type) {
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
    if (UNLIKELY(!ptr))
       return; 
    
    t_chunk_header *chunk = get_chunk_from_ptr(ptr);
    if (chunk->zone_type == ZONE_LARGE) {
        t_zone_header *zone = (t_zone_header *)((char *)chunk - sizeof(t_zone_header));
        remove_zone(zone);
        munmap(zone, zone->zone_size);
    } else
        chunk->free = 1;
}
