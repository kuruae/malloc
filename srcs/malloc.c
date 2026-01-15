#include "alloc.h"
#include "chunk_utils.h"
#include <sys/mman.h>

void *return_ptr(size_t size) {
    if (size <= get_tiny_max())
        return placeholder();
    else if (size <= get_small_max())
        return placeholder();
    else
        return placeholder();

    // void* ptr = mmap(
    //     NULL,
    //     size,
    //     PROT_READ | PROT_WRITE,
    //     MAP_PRIVATE | MAP_ANONYMOUS,
    //     -1,
    //     0
    //     );

    // ft_printf("\n%p\n", ptr);
    return NULL;
}

void *malloc(size_t size) {
    ft_printf("align: %d\n", ALIGNMENT);
    return_ptr(size);
    return (NULL);
}
