#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>
#include <stdalign.h>
#include <sys/mman.h>
#include "libft.h"


// zones
#define TINY_MAX 128
#define SMALL_MAX 1024

#define ALIGNMENT alignof(max_align_t)

void *malloc(size_t size);

#endif
