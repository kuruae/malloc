#ifndef ALLOC_H
#define ALLOC_H

#include <stdalign.h>
#include <sys/mman.h>
#include "libft.h"
#include "compiler_attrs.h"

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
	uint8_t                 free;
	uint8_t                 zone_type;
} __attribute__((aligned(ALIGNMENT))) t_chunk_header;

typedef struct s_zones {
	t_zone_header   *tiny;
	t_zone_header   *small;
	t_zone_header   *large;
} t_zones;

extern t_zones g_zones;


void    *malloc(size_t size) ATTR_HOT ATTR_MALLOC;
void    free(void *ptr) ATTR_HOT;
void    *realloc(void *ptr, size_t size) ATTR_HOT ATTR_MALLOC;
void    show_alloc_mem(void) ATTR_COLD;

#endif
