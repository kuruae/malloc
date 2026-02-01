#include "alloc.h"

extern t_thread_safety g_thread_safety;

static void free_zone_list(t_zone_header *zone) {
    while (zone) {
        t_zone_header *next = zone->next;
        munmap(zone, zone->zone_size);
        zone = next;
    }
}

static void cleanup_thread_zones() {
    free_zone_list(g_thread_zones.tiny);
    free_zone_list(g_thread_zones.small);
    free_zone_list(g_thread_zones.large);
    
    g_thread_zones.tiny = NULL;
    g_thread_zones.small = NULL;
    g_thread_zones.large = NULL;
}

static void init_cleanup_key() {
    pthread_key_create(&g_thread_safety.cleanup_key, cleanup_thread_zones);
}

void register_thread_cleanup() {
    pthread_once(&g_thread_safety.cleanup_once, init_cleanup_key);
    pthread_setspecific(g_thread_safety.cleanup_key, (void*)1);
}