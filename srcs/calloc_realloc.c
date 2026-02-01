#include "alloc.h"
#include "chunk_utils.h"

void *realloc(void *ptr, size_t size) {
	if (!ptr)
		return malloc(size);

	if (size == 0) {
		free(ptr);
		return NULL;
	}

	t_chunk_header *old_chunk = get_chunk_from_ptr(ptr);
	size_t old_size = old_chunk->size;

	void *new_ptr = malloc(size);
	if (!new_ptr)
		return NULL;

	size_t copy_size = (old_size < size) ? old_size : size;
	ft_memcpy(new_ptr, ptr, copy_size);

	free(ptr);

	return new_ptr;
}

void *calloc(size_t nmemb, size_t size) {
    size_t total = nmemb * size;

    if (nmemb && total / nmemb != size)
        return NULL;

    void *ptr = malloc(total);
    if (ptr)
        ft_memset(ptr, 0, total);
    return ptr;
}