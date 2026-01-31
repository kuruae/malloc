#include "alloc.h"
#include "ft_printf.h"
#include "size_utils.h"
#include "zones.h"

t_zones g_zones = {NULL, NULL, NULL};

// Returns total mmap size for large allocation (zone + chunk + data, page-aligned)
static size_t get_large_alloc_size(size_t size) {
    size_t total = sizeof(t_zone_header) + sizeof(t_chunk_header) + size;
    size_t page_size = get_system_page_size();

    return (total + (page_size - 1)) & ~(page_size - 1);
}

// Allocates large block in dedicated mmap'd zone
static void *alloc_large(size_t size) {
    size_t zone_size = get_large_alloc_size(size);

    t_zone_header *zone = create_zone(zone_size, ZONE_LARGE);
    if (!zone)
        return NULL;

    add_zone(zone);

    t_chunk_header *chunk = get_first_chunk(zone);
    chunk->next = NULL;
    chunk->size = size;
    chunk->free = 0;

    return get_ptr_from_chunk(chunk);
}

static void *alloc_tiny(size_t size) {
	t_zone_header *zone = g_zones.tiny;
	
	while (zone) {
		t_chunk_header *chunk = find_free_chunk(zone, size);
		if (chunk) {
			chunk->free = 0;
			return get_ptr_from_chunk(chunk);
		}
		
		void *ptr = carve_chunk(zone, size);
		if (ptr)
			return ptr;
		
		zone = zone->next;
	}
	
	zone = create_zone(get_zone_size(get_tiny_max()), ZONE_TINY);
	if (!zone)
		return NULL;
	
	add_zone(zone);
	return carve_chunk(zone, size);
}

static void *alloc_small(size_t size) {
	t_zone_header *zone = g_zones.small;
	
	while (zone) {
		t_chunk_header *chunk = find_free_chunk(zone, size);
		if (chunk) {
			chunk->free = 0;
			return get_ptr_from_chunk(chunk);
		}
		
		void *ptr = carve_chunk(zone, size);
		if (ptr)
			return ptr;
		
		zone = zone->next;
	}
	
	zone = create_zone(get_zone_size(get_small_max()), ZONE_SMALL);
	if (!zone)
		return NULL;
	
	add_zone(zone);
	return carve_chunk(zone, size);
}

/**
 * @brief Main malloc implementation
 *
 * Dispatches to appropriate allocator based on size:
 * - TINY: size <= page_size/32
 * - SMALL: size <= page_size/4
 * - LARGE: size > page_size/4
 *
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory, or NULL on failure
 */
void *malloc(size_t size) {
    if (size == 0)
        return NULL;

    ft_printf("entered malloc!\n");
    if (size <= get_tiny_max())
        return alloc_tiny(size);
    else if (size <= get_small_max())
        return alloc_small(size);
    else
        return alloc_large(size);
}

void *realloc(void *ptr, size_t size) {
    (void)ptr;
    (void)size;
    return NULL;
}
