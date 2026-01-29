#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>
#include <stdalign.h>
#include <sys/mman.h>
#include "libft.h"

#define ALIGNMENT alignof(max_align_t)

typedef enum e_zone_type {
    ZONE_TINY,
    ZONE_SMALL,
    ZONE_LARGE
} t_zone_type;

typedef struct s_zone_header {
    struct s_zone_header    *next;
    size_t                  zone_size;
    t_zone_type             type;
    void                    *break_ptr;
} __attribute__((aligned(ALIGNMENT))) t_zone_header;

typedef struct s_chunk_header {
    struct s_chunk_header   *next;
    size_t                  size;
    int                     free;
} __attribute__((aligned(ALIGNMENT))) t_chunk_header;

typedef struct s_zones {
    t_zone_header   *tiny;
    t_zone_header   *small;
    t_zone_header   *large;
} t_zones;

extern t_zones g_zones;


void    *malloc(size_t size);
void    free(void *ptr);
void    *realloc(void *ptr, size_t size);

#endif
