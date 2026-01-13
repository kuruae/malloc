#include "chunk_utils.h"

static size_t get_system_page_size() {
    static size_t page_size = 0;

    if (page_size > 0)
        return page_size;

    #ifdef __APPLE__
    page_size = (size_t)getpagesize();
    #else
    page_size = (size_t)sysconf(_SC_PAGESIZE);
    #endif

    return page_size;
}

size_t get_tiny_max() {
    static size_t tiny = 0;
    if (tiny == 0)
        tiny = get_system_page_size() / TINY_RATIO;
    return tiny;
}

size_t get_small_max() {
    static size_t small = 0;
    if (small == 0)
        small = get_system_page_size() / SMALL_RATIO;
    return small;
    }

    /**
 * @brief Calculates the aligned zone size for a memory pool.
 *
 * Computes the total size needed for a memory zone that can hold at least
 * MIN_ALLOC_COUNT allocations of the specified type size
 *
 * @param max_type_size Max size of objects that will be allocated in this zone
 * @return Size of the zone in bytes, aligned to system page size
 *
 * @note The returned size includes space for headers
 * @note Result is always a multiple of the system page size for efficient mmap usage
 */
size_t get_zone_size(size_t max_type_size) {
    size_t page_size = get_system_page_size();
    size_t header_size = sizeof(void*); // change this when i'll have my header struct
    size_t needed = (max_type_size + header_size) * MIN_ALLOC_COUNT;

    return (needed + (page_size - 1)) & ~(page_size - 1);
}
