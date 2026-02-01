#include "alloc.h"
#include "compiler_attrs.h"
#include "size_utils.h"
#include "zones.h"
#include "chunk_utils.h"
#include "thread_cleanup.h"

static size_t get_large_alloc_size(size_t size) {
	size_t total = sizeof(t_zone_header) + sizeof(t_chunk_header) + size;
	size_t page_size = get_system_page_size();

	return (total + (page_size - 1)) & ~(page_size - 1);
}

static void *alloc_large(size_t size) {
	size_t zone_size = get_large_alloc_size(size);

	t_zone_header *zone = create_zone(zone_size, ZONE_LARGE);
	if (UNLIKELY(!zone))
		return NULL;

	add_zone(zone);

	t_chunk_header *chunk = get_first_chunk(zone);
	chunk->size = size;
	chunk->free = 0;

	zone->break_ptr = (char *)chunk + sizeof(t_chunk_header) + size;

	return get_ptr_from_chunk(chunk);
}

static void *alloc_tiny(size_t size) {
	t_zone_header *zone = g_thread_zones.tiny;

	while (zone) {
		t_chunk_header *chunk = find_free_chunk(zone, size);
		if (LIKELY(chunk != NULL)) {
			chunk->free = 0;
			return get_ptr_from_chunk(chunk);
		}

		void *ptr = carve_chunk(zone, size);
		if (LIKELY(ptr != NULL))
			return ptr;

		zone = zone->next;
	}

	zone = create_zone(get_zone_size(get_tiny_max()), ZONE_TINY);
	if (UNLIKELY(!zone))
		return NULL;

	add_zone(zone);
	return carve_chunk(zone, size);
}

static void *alloc_small(size_t size) {
	t_zone_header *zone = g_thread_zones.small;

	while (zone) {
		t_chunk_header *chunk = find_free_chunk(zone, size);
		if (LIKELY(chunk != NULL)) {
			chunk->free = 0;
			return get_ptr_from_chunk(chunk);
		}

		void *ptr = carve_chunk(zone, size);
		if (LIKELY(ptr != NULL))
			return ptr;

		zone = zone->next;
	}

	zone = create_zone(get_zone_size(get_small_max()), ZONE_SMALL);
	if (UNLIKELY(!zone))
		return NULL;

	add_zone(zone);
	return carve_chunk(zone, size);
}

void *malloc(size_t size) {
    static __thread int cleanup_registered = 0;
    if (UNLIKELY(!cleanup_registered)) {
        register_thread_cleanup();
        cleanup_registered = 1;
    }
    
	if (UNLIKELY(size == 0))
		return NULL;

	if (UNLIKELY(size > (size_t)-1 - sizeof(t_zone_header) - sizeof(t_chunk_header)))
		return NULL;

	if (size <= get_tiny_max())
		return alloc_tiny(size);
	else if (size <= get_small_max())
		return alloc_small(size);
	else
		return alloc_large(size);
}
