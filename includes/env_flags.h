#ifndef ENV_FLAGS_H
#define ENV_FLAGS_H

#include "alloc.h"

/*
 * Each of these flags match an environment variable, in all-uppercase.
 *
 * M_ALLOC_LOGS     =   0: no logs, normal (default)
 *                      1: malloc() functions and free() will print logs to the
 * standard output. 2: same as 1 but also logs the number of times nmap was
 * called and the- total allocated size at the end of the program.
 *
 * M_CHECK_WILD_PTR =   0: turns pointer integrity checks off, making free()
 * faster (up to O(n)) 1: keeps free from reading foreign pointers (default)
 *
 * M_HALT_ON_EXIT   =   0: normal behavior (default)
 *                  =   1: keeps memory/process in a frozen state using pause()
 * at exit
 *
 * M_HALT_ON_ERROR  =   0: normal behavior (default)
 *                  =   1: keeps memory/process in a frozen state using pause()
 * at the first error
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
  uint8_t m_halt_on_exit : 1;
  uint8_t m_halt_on_error : 1;
  uint8_t m_fill_on_free : 1;
  uint8_t m_color : 1;
  uint8_t : 1;
} t_env_flags;

void constructor();

// ===== GETTERS

static inline uint8_t env_get_alloc_logs(const t_env_flags *flags) {
  return flags->m_alloc_logs;
}

static inline int env_is_check_wild_ptr(const t_env_flags *flags) {
  return flags->m_check_wild_ptr;
}

static inline int env_is_halt_on_exit(const t_env_flags *flags) {
  return flags->m_halt_on_exit;
}

static inline int env_is_halt_on_error(const t_env_flags *flags) {
  return flags->m_halt_on_error;
}

static inline int env_is_fill_on_free(const t_env_flags *flags) {
  return flags->m_fill_on_free;
}

static inline int env_is_color(const t_env_flags *flags) {
  return flags->m_color;
}

#endif
