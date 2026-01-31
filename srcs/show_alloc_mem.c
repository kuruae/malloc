#include "alloc.h"
#include "zones.h"
#include "ft_printf.h"

// move to next chunk in memory
static inline t_chunk_header *next_chunk(t_chunk_header *chunk) {
	return (t_chunk_header *)((char *)chunk + sizeof(t_chunk_header) + chunk->size);
}

// print all allocated chunks in a single zone, returns bytes printed
static size_t print_zone_chunks(t_zone_header *zone) {
	size_t total = 0;
	t_chunk_header *chunk = get_first_chunk(zone);
	
	while ((void *)chunk < zone->break_ptr) {
		if (!chunk->free) {
			void *start = get_ptr_from_chunk(chunk);
			void *end = (char *)start + chunk->size - 1;
			ft_printf("%p - %p : %u bytes\n", start, end, (unsigned int)chunk->size);
			total += chunk->size;
		}
		chunk = next_chunk(chunk);
	}
	
	return total;
}

// check if zone has any allocated chunks
static int zone_has_allocations(t_zone_header *zone) {
	t_chunk_header *chunk = get_first_chunk(zone);
	
	while ((void *)chunk < zone->break_ptr) {
		if (!chunk->free)
			return 1;
		chunk = next_chunk(chunk);
	}
	
	return 0;
}

// shows all allocated chunks in a zone list and returns total bytes
static size_t show_zone_list(t_zone_header *zone, const char *type) {
	size_t total = 0;
	int zone_type_printed = 0;

	while (zone) {
		if (!zone_has_allocations(zone)) {
			zone = zone->next;
			continue;
		}
		
		if (!zone_type_printed) {
			ft_printf("%s : %p\n", type, (void *)zone);
			zone_type_printed = 1;
		}
		
		total += print_zone_chunks(zone);
		
		zone = zone->next;
	}
	
	return total;
}

void show_alloc_mem(void) {
	size_t total = 0;
	
	total += show_zone_list(g_zones.tiny, "TINY");
	total += show_zone_list(g_zones.small, "SMALL");
	total += show_zone_list(g_zones.large, "LARGE");
	
	ft_printf("Total : %u bytes\n", (unsigned int)total);
}
