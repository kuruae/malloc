#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H

#include "env_flags.h"

#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_RESET   "\033[0m"

static inline const char *color_error(void) {
  return env_is_color(env_flags_singleton()) ? COLOR_RED : "";
}

static inline const char *color_success(void) {
  return env_is_color(env_flags_singleton()) ? COLOR_GREEN : "";
}

static inline const char *color_info(void) {
  return env_is_color(env_flags_singleton()) ? COLOR_CYAN : "";
}

static inline const char *color_warning(void) {
  return env_is_color(env_flags_singleton()) ? COLOR_YELLOW : "";
}

static inline const char *color_reset(void) {
  return env_is_color(env_flags_singleton()) ? COLOR_RESET : "";
}

#endif
