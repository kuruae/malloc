#include "env_flags.h"
#include "libft.h"
#include <stdint.h>
#include <stdlib.h>

void __attribute__((constructor)) constructor();

// ===== SETTERS

static inline void env_set_alloc_logs(t_env_flags *flags, uint8_t level) {
  level = level < 3 ? level : 2;
  flags->m_alloc_logs = level & 0x3;
}

static inline void env_set_check_wild_ptr(t_env_flags *flags, int enabled) {
  flags->m_check_wild_ptr = enabled ? 1 : 0;
}

static inline void env_set_halt_on_exit(t_env_flags *flags, int enabled) {
  flags->m_halt_on_exit = enabled ? 1 : 0;
}

static inline void env_set_halt_on_error(t_env_flags *flags, int enabled) {
  flags->m_halt_on_error = enabled ? 1 : 0;
}

static inline void env_set_fill_on_free(t_env_flags *flags, int enabled) {
  flags->m_fill_on_free = enabled ? 1 : 0;
}

static inline void env_set_color(t_env_flags *flags, int enabled) {
  flags->m_color = enabled ? 1 : 0;
}

static uint8_t atoi_u8(const char *p) {
  uint8_t v = 0;
  while (*p) {
    v = v * 10 + *p - '0';
    p++;
  }
  return v;
}

t_env_flags *env_flags_singleton() {
  static t_env_flags instance = {0, 1, 0, 0, 0, 0};
  return &instance;
}

void print_env_flags() {}

void constructor() {
  ft_putendl_fd("this is a test", 1);

  char *env_ptr = getenv("M_ALLOC_LOGS");
  if (env_ptr)
    env_set_alloc_logs(env_flags_singleton(), atoi_u8(env_ptr));

  env_ptr = getenv("M_CHECK_WILD_PTR");
  if (env_ptr)
    env_set_check_wild_ptr(env_flags_singleton(), atoi_u8(env_ptr));

  env_ptr = getenv("M_HALT_ON_EXIT");
  if (env_ptr)
    env_set_halt_on_exit(env_flags_singleton(), atoi_u8(env_ptr));

  env_ptr = getenv("M_HALT_ON_ERROR");
  if (env_ptr)
    env_set_halt_on_error(env_flags_singleton(), atoi_u8(env_ptr));

  env_ptr = getenv("M_FILL_ON_FREE");
  if (env_ptr)
    env_set_fill_on_free(env_flags_singleton(), atoi_u8(env_ptr));
  env_ptr = getenv("M_COLOR");
  if (env_ptr)
    env_set_color(env_flags_singleton(), atoi_u8(env_ptr));

  if (env_get_alloc_logs(env_flags_singleton()) > 0)
    print_env_flags();
}
