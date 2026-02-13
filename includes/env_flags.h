#ifndef ENV_FLAG_H
#define ENV_FLAG_H

#include "alloc.h"

/*
 * Each of these flags match an environment variable, in all-uppercase.
 *
 * M_ALLOC_LOGS     =   0: no logs, normal (default)
 *                      1: malloc() functions and free() will print logs to the
 * standard output. 2: same as 1 but also logs the number of times nmap was
 * called and the- total allocated size at the end of the program.
 *
 * M_CHECK_WILD_PTR =   0: O(1) free() - no zone validation, no coalescing
 *                         (fast but fragments, undefined behavior on wild ptrs)
 *                      1: O(n) free() - validates pointers, enables coalescing
 *                         (default, safe mode)
 *
 * M_CHECK_LEVEL    =   0: warn only on errors (default)
 *                      1: abort on serious errors (double-free, wild pointers)
 *                      2: paranoid mode with extra validation
 *
 * M_FILL_ON_FREE   =   0: normal behavior (default)
 *                  =   1: fills address to overwrite garbage data
 *
 * M_COLOR          =   0: normal behavior (default)
 *                  =   1: changes all malloc() functions output to colored
 * texts
 *
 * Putting adequate values is the user's responsability
 * as the parsing is kept minimal for performance issues
 */
typedef struct s_env_flags {
  uint8_t m_alloc_logs : 2;
  uint8_t m_check_wild_ptr : 1;
  uint8_t m_check_level : 2;
  uint8_t m_fill_on_free : 1;
  uint8_t m_color : 1;
  uint8_t : 1;
} t_env_flags;

void constructor();
t_env_flags *env_flags_singleton();

// ===== GETTERS

static inline uint8_t env_get_alloc_logs(const t_env_flags *flags) {
  return flags->m_alloc_logs;
}

static inline int env_is_check_wild_ptr(const t_env_flags *flags) {
  return flags->m_check_wild_ptr;
}

static inline uint8_t env_get_check_level(const t_env_flags *flags) {
  return flags->m_check_level;
}

static inline int env_is_fill_on_free(const t_env_flags *flags) {
  return flags->m_fill_on_free;
}

static inline int env_is_color(const t_env_flags *flags) {
  return flags->m_color;
}

#endif
