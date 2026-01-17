#include "alloc.h"

t_zones g_zones = {NULL, NULL, NULL};

/**
 * @brief Creates a new zone via mmap
 *
 * Allocates a memory region of the specified size using mmap.
 * The zone header is placed at the start of the region.
 *
 * @param size Total size of the zone (including headers)
 * @param type Type of zone (TINY, SMALL, or LARGE)
 * @return Pointer to the zone header, or NULL on failure
 */
t_zone_header *create_zone(size_t size, t_zone_type type) {
    void *ptr = mmap(
        NULL,
        size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    if (ptr == MAP_FAILED)
        return NULL;

    t_zone_header *zone = (t_zone_header *)ptr;
    zone->next = NULL;
    zone->zone_size = size;
    zone->type = type;

    return zone;
}

/**
 * @brief Adds a zone to the appropriate zone list
 *
 * Inserts the zone at the head of the corresponding list.
 *
 * @param zone Pointer to the zone to add
 */
void add_zone(t_zone_header *zone) {
    if (!zone)
        return;

    t_zone_header **list = NULL;

    if (zone->type == ZONE_TINY)
        list = &g_zones.tiny;
    else if (zone->type == ZONE_SMALL)
        list = &g_zones.small;
    else
        list = &g_zones.large;

    zone->next = *list;
    *list = zone;
}

/**
 * @brief Gets a pointer to the first chunk in a zone
 *
 * The first chunk starts immediately after the zone header.
 *
 * @param zone Pointer to the zone
 * @return Pointer to the first chunk header
 */
t_chunk_header *get_first_chunk(t_zone_header *zone) {
    if (!zone)
        return NULL;
    return (t_chunk_header *)((char *)zone + sizeof(t_zone_header));
}

/**
 * @brief Gets the user data pointer from a chunk header
 *
 * User data starts immediately after the chunk header.
 *
 * @param chunk Pointer to the chunk header
 * @return Pointer to user-accessible memory
 */
void *get_ptr_from_chunk(t_chunk_header *chunk) {
    if (!chunk)
        return NULL;
    return (void *)((char *)chunk + sizeof(t_chunk_header));
}

/**
 * @brief Gets the chunk header from a user data pointer
 *
 * Reverse of get_ptr_from_chunk - used in free().
 *
 * @param ptr User data pointer (as returned by malloc)
 * @return Pointer to the chunk header
 */
t_chunk_header *get_chunk_from_ptr(void *ptr) {
    if (!ptr)
        return NULL;
    return (t_chunk_header *)((char *)ptr - sizeof(t_chunk_header));
}
