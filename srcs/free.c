#include "alloc.h"
#include "chunk_utils.h"
#include "color_utils.h"
#include "env_flags.h"
#include "libft.h"
#include "log_utils.h"
#include "zones.h"
#include <stdlib.h>

static t_zone_header *find_zone_for_ptr_in_list(const void *ptr,
                                                t_zone_header *zone) {
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

static t_zone_header *find_zone_for_ptr_any(const void *ptr,
                                            t_zone_type *out_type) {
  t_zone_header *zone;

  zone = find_zone_for_ptr_in_list(ptr, g_thread_zones.tiny);
  if (zone) {
    *out_type = ZONE_TINY;
    return zone;
  }
  zone = find_zone_for_ptr_in_list(ptr, g_thread_zones.small);
  if (zone) {
    *out_type = ZONE_SMALL;
    return zone;
  }
  zone = find_zone_for_ptr_in_list(ptr, g_thread_zones.large);
  if (zone) {
    *out_type = ZONE_LARGE;
    return zone;
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

  log_free(ptr);

  t_env_flags *flags = env_flags_singleton();
  uint8_t check_level = env_get_check_level(flags);

  if (UNLIKELY((uintptr_t)ptr % ALIGNMENT) != 0) {
    handle_error("free(): misaligned pointer in free()", check_level);
    return;
  }

  t_zone_type zone_type;
  t_zone_header *zone = find_zone_for_ptr_any(ptr, &zone_type);
  if (UNLIKELY(!zone)) {
    handle_error("free(): wild or foreign pointer", check_level);
    return;
  }

  t_chunk_header *chunk = get_chunk_from_ptr(ptr);

  if (UNLIKELY(chunk->free == 1)) {
    handle_error("free(): double free detected", check_level);
    return;
  }

  if (zone_type == ZONE_LARGE) {
    if (env_is_fill_on_free(flags))
      ft_bzero(ptr, chunk->size);
    remove_zone(zone);
    munmap(zone, zone->zone_size);
  } else {
    chunk->free = 1;
    if (env_is_fill_on_free(flags))
      ft_bzero(ptr, chunk->size);
    coalesce_forward(zone, chunk);
  }
}
