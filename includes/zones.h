#ifndef ZONES_H
#define ZONES_H

#include "alloc.h"
#include "compiler_attrs.h"
#include <stddef.h>

t_zone_header   *create_zone(size_t size, t_zone_type type);
void            add_zone(t_zone_header *zone) ATTR_NONNULL(1);
void            remove_zone(t_zone_header *zone) ATTR_NONNULL(1);
t_zone_header   *find_zone_for_ptr(const void *ptr, t_zone_type type);


#endif
