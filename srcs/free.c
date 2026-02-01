#include "alloc.h"
#include "compiler_attrs.h"
#include "libft.h"
#include "zones.h"
#include "chunk_utils.h"
#include <stdio.h>

t_zone_header *find_zone_for_ptr(const void *ptr, t_zone_type type) {
	t_zone_header *zone;
	if (type == ZONE_TINY)
		zone = g_zones.tiny;
	else if (type == ZONE_SMALL)
		zone = g_zones.small;
	else
		zone = g_zones.large;

	while (zone) {
    void *zone_end = (char *)zone + zone->zone_size;
    void *first_valid_ptr = (char *)zone + sizeof(t_zone_header) + sizeof(t_chunk_header);

    if (ptr >= first_valid_ptr && ptr < zone_end)
        return zone;
    zone = zone->next;
}
	return NULL;
}

void free(void *ptr) {
    if (UNLIKELY(!ptr))
       return;

    if (UNLIKELY((uintptr_t)ptr % ALIGNMENT) != 0) {
        ft_putendl_fd("free(): misaligned pointer in free()", 2);
        return;
    }
    
    t_zone_header *zone = NULL;
        if (!(zone = find_zone_for_ptr(ptr, ZONE_TINY)) &&
            !(zone = find_zone_for_ptr(ptr, ZONE_SMALL)) &&
            !(zone = find_zone_for_ptr(ptr, ZONE_LARGE))) {
                ft_putendl_fd("free(): wild or foreign pointer", 2);
                return;
        }
    
    t_chunk_header *chunk = get_chunk_from_ptr(ptr);

    if (UNLIKELY(chunk->free == 1)) {
        ft_putendl_fd("free(): double free detected", 2);
        return;
    }
    
    
    if (chunk->zone_type == ZONE_LARGE) {
        t_zone_header *zone = (t_zone_header *)((char *)chunk - sizeof(t_zone_header));
        remove_zone(zone);
        munmap(zone, zone->zone_size);
    } else
        chunk->free = 1;
}
