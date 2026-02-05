#ifndef ALLOC_H
#define ALLOC_H

#define _DEFAULT_SOURCE
#include <stdalign.h>
#include <stdint.h>
#include <sys/mman.h>
#include <pthread.h>
#include "libft.h"
#include "compiler_attrs.h"

#if defined(MAP_ANONYMOUS)
    // use MAP_ANONYMOUS
#elif defined(MAP_ANON)
    #define MAP_ANONYMOUS MAP_ANON
#else
    #error "Neither MAP_ANONYMOUS nor MAP_ANON is defined"
#endif

#define ALIGNMENT alignof(max_align_t)

typedef enum e_zone_type {
	ZONE_TINY,
	ZONE_SMALL,
	ZONE_LARGE
} t_zone_type;

typedef struct s_zone_header {
	struct s_zone_header    *next;
	size_t                  zone_size;
	t_zone_type             type;
	void                    *break_ptr;
} __attribute__((aligned(ALIGNMENT))) t_zone_header;

typedef struct s_chunk_header {
	size_t                  size;
	uint8_t                 free;
	uint8_t                 zone_type;
} __attribute__((aligned(ALIGNMENT))) t_chunk_header;

typedef struct s_zones {
	t_zone_header   *tiny;
	t_zone_header   *small;
	t_zone_header   *large;
} t_zones;

extern __thread t_zones g_thread_zones;

typedef struct s_thread_safety {
    pthread_key_t   cleanup_key;
    pthread_once_t  cleanup_once;
} t_thread_safety;

/*
 * Each of these flags match an environment variable, in all-uppercase.
 * 
 * M_ALLOC_LOGS     =   0: no logs, normal (default)
 *                      1: malloc() functions and free() will print logs to the standard output.
 *                      2: same as 1 but also logs the number of times nmap was called and the-
 *                          total allocated size at the end of the program.
 *  
 * M_CHECK_WILD_PTR =   0: turns pointer integrity checks off, making free() faster (up to O(n)) 
 *                      1: keeps free from reading foreign pointers (default)
 *                     
 * M_HALT_ON_EXIT   =   0: normal behavior (default)
 *                  =   1: keeps memory/process in a frozen state using pause() at exit
 * 
 * M_HALT_ON_ERROR  =   0: normal behavior (default)  
 *                  =   1: keeps memory/process in a frozen state using pause() at the first error
 * 
 * M_FILL_ON_FREE   =   0: normal behavior (default)
 *                  =   1: fills address to overwrite garbage data
 *
 * M_COLOR          =   0: normal behavior (default)
 *                  =   1: changes all malloc() functions output to colored texts
 */
typedef struct s_env_flags {
    uint8_t        m_alloc_logs        : 2;
    uint8_t        m_check_wild_ptr    : 1;
    uint8_t        m_halt_on_exit      : 1;
    uint8_t        m_halt_on_error     : 1;
    uint8_t        m_fill_on_free      : 1;
    uint8_t        m_color             : 1;
    uint8_t                            : 1;
} t_env_flags;

void    *malloc(size_t size) ATTR_HOT ATTR_MALLOC;
void    free(void *ptr) ATTR_HOT;
void    *realloc(void *ptr, size_t size) ATTR_HOT ATTR_MALLOC;
void    show_alloc_mem(void) ATTR_COLD;

#endif
