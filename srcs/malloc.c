#include "alloc.h"

void *malloc(size_t size) {
    ft_printf("align: %d\n", ALIGNMENT);
    if (size != 0)
        return (0);

    return (NULL);
}
