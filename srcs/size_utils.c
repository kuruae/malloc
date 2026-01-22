#include "alloc.h"
#include "size_utils.h"

size_t get_system_page_size() {
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

// Calculates page-aligned zone size for MIN_ALLOC_COUNT allocations of max_type_size
size_t get_zone_size(size_t max_type_size) {
    size_t page_size = get_system_page_size();
    size_t header_size = sizeof(t_zone_header) + sizeof(t_chunk_header);
    size_t needed = (max_type_size + header_size) * MIN_ALLOC_COUNT;

    return (needed + (page_size - 1)) & ~(page_size - 1);
}
