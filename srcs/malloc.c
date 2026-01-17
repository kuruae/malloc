#include "alloc.h"
#include "size_utils.h"
#include "zones.h"

/**
 * @brief Calculates total size needed for a large allocation
 *
 * Includes zone header + chunk header + user data, aligned to page size.
 *
 * @param size Requested user data size
 * @return Total mmap size needed
 */
static size_t get_large_alloc_size(size_t size) {
    size_t total = sizeof(t_zone_header) + sizeof(t_chunk_header) + size;
    size_t page_size = get_system_page_size();

    return (total + (page_size - 1)) & ~(page_size - 1);
}

/**
 * @brief Allocates a large block (> SMALL threshold)
 *
 * Each large allocation gets its own mmap'd zone.
 *
 * @param size Requested size
 * @return Pointer to user data, or NULL on failure
 */
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

/** 
 * @brief Allocates memory from tiny zone (TODO)
 *
 * @param size Requested size
 * @return Pointer to user data, or NULL on failure
 */
static void *alloc_tiny(size_t size) {
    (void)size;
    return NULL;
}

/**
 * @brief Allocates memory from small zone (TODO)
 *
 * @param size Requested size
 * @return Pointer to user data, or NULL on failure
 */
static void *alloc_small(size_t size) {
    (void)size;
    return NULL;
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

    if (size <= get_tiny_max())
        return alloc_tiny(size);
    else if (size <= get_small_max())
        return alloc_small(size);
    else
        return alloc_large(size);
}

/**
 * @brief Frees allocated memory
 *
 * @param ptr Pointer to memory to free
 */
void free(void *ptr) {
    (void)ptr;
    // TODO
}

/**
 * @brief Reallocates memory to a new size (TODO)
 *
 * @param ptr Pointer to existing allocation
 * @param size New size
 * @return Pointer to reallocated memory, or NULL on failure
 */
void *realloc(void *ptr, size_t size) {
    (void)ptr;
    (void)size;
    return NULL;
}
