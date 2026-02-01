#include "alloc.h"
#include "chunk_utils.h"
#include "compiler_attrs.h"

t_chunk_header *find_free_chunk(t_zone_header *zone, size_t size) {
	t_chunk_header *it = get_first_chunk(zone);

	while (it != NULL && (void *)it < zone->break_ptr) {
		if (UNLIKELY(it->free == 1 && it->size >= size))
			return it;
		it = next_chunk(it);
	}
	return NULL;
}

static inline size_t align_size(size_t size) {
	return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

void *carve_chunk(t_zone_header *zone, size_t size) {
	size_t aligned_size = align_size(size);
	size_t needed = aligned_size + sizeof(t_chunk_header);

	if (UNLIKELY((char *)zone->break_ptr + needed > (char *)zone + zone->zone_size))
		return NULL;

	t_chunk_header *new_chunk = (t_chunk_header *)zone->break_ptr;

	new_chunk->next = NULL;
	new_chunk->size = aligned_size;
	new_chunk->free = 0;
	new_chunk->zone_type = zone->type;
	
	zone->break_ptr = (char *)zone->break_ptr + needed;

	return get_ptr_from_chunk(new_chunk);
}
