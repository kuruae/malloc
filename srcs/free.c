#include "alloc.h"
#include "chunk_utils.h"
#include "color_utils.h"
#include "env_flags.h"
#include "libft.h"
#include "zones.h"
#include <stdlib.h>

t_zone_header *find_zone_for_ptr(const void *ptr, t_zone_type type) {
  t_zone_header *zone;
  if (type == ZONE_TINY)
    zone = g_thread_zones.tiny;
  else if (type == ZONE_SMALL)
    zone = g_thread_zones.small;
  else
    zone = g_thread_zones.large;

  while (zone) {
    void *zone_end = (char *)zone + zone->zone_size;
    void *first_valid_ptr =
        (char *)zone + sizeof(t_zone_header) + sizeof(t_chunk_header);

    if (ptr >= first_valid_ptr && ptr < zone_end)
      return zone;
    zone = zone->next;
  }
  return NULL;
}

static inline void handle_error(const char *msg, uint8_t check_level) {
  ft_putstr_fd((char *)color_error(), 2);
  ft_putstr_fd((char *)msg, 2);
  ft_putendl_fd((char *)color_reset(), 2);
  if (check_level >= 1)
    abort();
}

void free(void *ptr) {
  if (UNLIKELY(!ptr))
    return;

  t_env_flags *flags = env_flags_singleton();
  uint8_t check_level = env_get_check_level(flags);

  if (UNLIKELY((uintptr_t)ptr % ALIGNMENT) != 0) {
    handle_error("free(): misaligned pointer in free()", check_level);
    return;
  }

  t_zone_header *zone = NULL;
  if (env_is_check_wild_ptr(flags)) {
    if (!(zone = find_zone_for_ptr(ptr, ZONE_TINY)) &&
        !(zone = find_zone_for_ptr(ptr, ZONE_SMALL)) &&
        !(zone = find_zone_for_ptr(ptr, ZONE_LARGE))) {
      handle_error("free(): wild or foreign pointer", check_level);
      return;
    }
  }

  t_chunk_header *chunk = get_chunk_from_ptr(ptr);

  if (UNLIKELY(chunk->free == 1)) {
    handle_error("free(): double free detected", check_level);
    return;
  }

  if (chunk->zone_type == ZONE_LARGE) {
    if (env_is_fill_on_free(flags))
      ft_bzero(ptr, chunk->size);
    t_zone_header *zone =
        (t_zone_header *)((char *)chunk - sizeof(t_zone_header));
    remove_zone(zone);
    munmap(zone, zone->zone_size);
  } else {
    chunk->free = 1;
    if (env_is_fill_on_free(flags))
      ft_bzero(ptr, chunk->size);
    coalesce_forward(zone, chunk);
  }
}
