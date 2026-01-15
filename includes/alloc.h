#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>
#include <stdalign.h>
#include <sys/mman.h>
#include "libft.h"

#define ALIGNMENT alignof(max_align_t)

typedef struct __attribute__((aligned(ALIGNMENT))) {
    struct t_header *next;
    size_t          size;
    int             free;
} t_header;

void *malloc(size_t size);

#endif
