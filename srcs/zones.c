#include "alloc.h"
#include <stddef.h>

t_zones g_zones = {NULL, NULL, NULL};

// Creates mmap'd zone with specified size and type
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
    zone->break_ptr = (char *)zone + sizeof(t_zone_header);

    return zone;
}

// Adds zone to head of appropriate linked list (tiny/small/large)
void add_zone(t_zone_header *zone) {
    if (!zone)
        return;

    t_zone_header **curr = NULL;

    if (zone->type == ZONE_TINY)
        curr = &g_zones.tiny;
    else if (zone->type == ZONE_SMALL)
        curr = &g_zones.small;
    else
        curr = &g_zones.large;

    zone->next = *curr;
    *curr = zone;
}

void remove_zone(t_zone_header *zone_to_remove) {
    if (!zone_to_remove)
        return;

    t_zone_header **curr;

    if (zone_to_remove->type == ZONE_TINY)
        curr = &g_zones.tiny;
    else if (zone_to_remove->type == ZONE_SMALL)
        curr = &g_zones.small;
    else
        curr = &g_zones.large;

    while (*curr) {
        if (*curr == zone_to_remove) {
            *curr = zone_to_remove->next;
            zone_to_remove->next = NULL;
            return;
        }
        curr = &((*curr)->next);
    }
}