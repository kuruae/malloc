#include "alloc.h"
#include "zones.h"
#include "chunk_utils.h"

t_zone_header *find_zone_for_ptr(const void *ptr, t_zone_type type) {
	t_zone_header *zone;
	if (type == ZONE_TINY)
		zone = g_thread_zones.tiny;
	else if (type == ZONE_SMALL)
		zone = g_thread_zones.small;
	else
		zone = g_thread_zones.large;

	while (zone) {
    void *zone_end = (char *)zone + zone->zone_size;
    void *first_valid_ptr = (char *)zone + sizeof(t_zone_header) + sizeof(t_chunk_header);

    if (ptr >= first_valid_ptr && ptr < zone_end)
        return zone;
    zone = zone->next;
}
	return NULL;
}

static inline int valid_ptr_check(void *ptr) {
    if (UNLIKELY(!ptr))
       return 1;

    if (UNLIKELY((uintptr_t)ptr % ALIGNMENT) != 0) {
        ft_putendl_fd("free(): misaligned pointer in free()", 2);
        return 1;
    }
    
    t_zone_header *zone = NULL;
        if (!(zone = find_zone_for_ptr(ptr, ZONE_TINY)) &&
            !(zone = find_zone_for_ptr(ptr, ZONE_SMALL)) &&
            !(zone = find_zone_for_ptr(ptr, ZONE_LARGE))) {
                ft_putendl_fd("free(): wild or foreign pointer", 2);
                return 1;
        }
   return 0; 
}

void free(void *ptr) {
    if (!valid_ptr_check(ptr))
        return ;
    
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
