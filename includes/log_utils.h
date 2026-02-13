#ifndef LOG_UTILS_H
#define LOG_UTILS_H

#include "color_utils.h"
#include "env_flags.h"
#include "ft_fprintf.h"
#include <stddef.h>
#include <pthread.h>

static inline void log_malloc(void *ptr, size_t size) {
  t_env_flags *flags = env_flags_singleton();
  
  if (env_get_alloc_logs(flags) < 1)
    return;
  
  ft_fprintf(2, "%s[MALLOC]%s size=%u -> %p (thread %p)\n",
             color_info(), color_reset(),
             (unsigned int)size, ptr, (void *)pthread_self());
}

static inline void log_free(void *ptr) {
  t_env_flags *flags = env_flags_singleton();
  
  if (env_get_alloc_logs(flags) < 1)
    return;
  
  ft_fprintf(2, "%s[FREE]%s   ptr=%p (thread %p)\n",
             color_info(), color_reset(),
             ptr, (void *)pthread_self());
}

static inline void log_realloc(void *old_ptr, size_t size, void *new_ptr) {
  t_env_flags *flags = env_flags_singleton();
  
  if (env_get_alloc_logs(flags) < 1)
    return;
  
  ft_fprintf(2, "%s[REALLOC]%s ptr=%p, size=%u -> %p (thread %p)\n",
             color_info(), color_reset(),
             old_ptr, (unsigned int)size, new_ptr, (void *)pthread_self());
}

#endif
