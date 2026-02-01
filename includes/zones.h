#ifndef ZONES_H
#define ZONES_H

#include "alloc.h"
#include <stddef.h>

t_zone_header   *create_zone(size_t size, t_zone_type type);
void            add_zone(t_zone_header *zone);
void            remove_zone(t_zone_header *zone);
t_zone_header   *find_zone_for_ptr(void *ptr, t_zone_type type);


#endif
