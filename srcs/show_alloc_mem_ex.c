#include "alloc.h"
#include "chunk_utils.h"
#include "color_utils.h"
#include "env_flags.h"
#include <unistd.h>

#define HEXDUMP_BYTES_PER_LINE 16
#define HEXDUMP_MAX_BYTES 64

static void print_hex_byte(unsigned char byte) {
	const char *hex = "0123456789abcdef";
	char buf[3];
	buf[0] = hex[byte >> 4];
	buf[1] = hex[byte & 0xF];
	buf[2] = ' ';
	write(STDERR_FILENO, buf, 3);
}

static void print_hex_offset(size_t offset) {
	const char *hex = "0123456789abcdef";
	char buf[7];
	buf[0] = ' ';
	buf[1] = ' ';
	buf[2] = hex[(offset >> 12) & 0xF];
	buf[3] = hex[(offset >> 8) & 0xF];
	buf[4] = hex[(offset >> 4) & 0xF];
	buf[5] = hex[offset & 0xF];
	buf[6] = ':';
	write(STDERR_FILENO, buf, 7);
}

static void print_hexdump_line(unsigned char *data, size_t offset, size_t len) {
	print_hex_offset(offset);
	write(STDERR_FILENO, " ", 1);
	
	for (size_t i = 0; i < HEXDUMP_BYTES_PER_LINE; i++) {
		if (i < len)
			print_hex_byte(data[i]);
		else
			write(STDERR_FILENO, "   ", 3);
		if (i == 7)
			write(STDERR_FILENO, " ", 1);
	}
	
	write(STDERR_FILENO, " ", 1);
	for (size_t i = 0; i < len; i++) {
		unsigned char c = data[i];
		if (c >= 32 && c <= 126)
			write(STDERR_FILENO, &c, 1);
		else
			write(STDERR_FILENO, ".", 1);
	}
	write(STDERR_FILENO, "\n", 1);
}

static void print_hexdump(void *ptr, size_t size) {
	size_t bytes_to_dump = size < HEXDUMP_MAX_BYTES ? size : HEXDUMP_MAX_BYTES;
	unsigned char *data = (unsigned char *)ptr;
	
	for (size_t offset = 0; offset < bytes_to_dump; offset += HEXDUMP_BYTES_PER_LINE) {
		size_t line_len = bytes_to_dump - offset;
		if (line_len > HEXDUMP_BYTES_PER_LINE)
			line_len = HEXDUMP_BYTES_PER_LINE;
		print_hexdump_line(data + offset, offset, line_len);
	}
	
	if (size > HEXDUMP_MAX_BYTES)
		ft_printf("  ... (%u more bytes)\n", (unsigned int)(size - HEXDUMP_MAX_BYTES));
}

typedef struct s_zone_stats {
	size_t total_bytes;
	size_t used_bytes;
	size_t free_bytes;
	unsigned int used_chunks;
	unsigned int free_chunks;
} t_zone_stats;

static void calculate_zone_stats(t_zone_header *zone, t_zone_stats *stats) {
	stats->total_bytes = 0;
	stats->used_bytes = 0;
	stats->free_bytes = 0;
	stats->used_chunks = 0;
	stats->free_chunks = 0;
	
	while (zone) {
		stats->total_bytes += zone->zone_size;
		
		t_chunk_header *chunk = get_first_chunk(zone);
		while ((void *)chunk < zone->break_ptr) {
			if (chunk->free) {
				stats->free_bytes += chunk->size;
				stats->free_chunks++;
			} else {
				stats->used_bytes += chunk->size;
				stats->used_chunks++;
			}
			chunk = next_chunk(chunk);
		}
		
		zone = zone->next;
	}
}

static void print_chunk_details(t_chunk_header *chunk, int show_hexdump) {
	void *start = get_ptr_from_chunk(chunk);
	void *end = (char *)start + chunk->size - 1;
	
	ft_printf("  %p - %p : %u bytes", start, end, (unsigned int)chunk->size);
	
	if (show_hexdump && chunk->size > 0) {
		ft_printf("\n");
		print_hexdump(start, chunk->size);
	} else {
		ft_printf("\n");
	}
}

static size_t print_zone_chunks_ex(t_zone_header *zone, int show_hexdump) {
	size_t total = 0;
	t_chunk_header *chunk = get_first_chunk(zone);

	while ((void *)chunk < zone->break_ptr) {
		if (!chunk->free) {
			print_chunk_details(chunk, show_hexdump);
			total += chunk->size;
		}
		chunk = next_chunk(chunk);
	}

	return total;
}

static int zone_has_allocations(t_zone_header *zone) {
	t_chunk_header *chunk = get_first_chunk(zone);

	while ((void *)chunk < zone->break_ptr) {
		if (!chunk->free)
			return 1;
		chunk = next_chunk(chunk);
	}

	return 0;
}

static size_t show_zone_list_ex(t_zone_header *zone, const char *type, int show_hexdump) {
	size_t total = 0;
	int zone_type_printed = 0;
	t_zone_stats stats;

	calculate_zone_stats(zone, &stats);

	if (stats.used_chunks == 0)
		return 0;

	while (zone) {
		if (!zone_has_allocations(zone)) {
			zone = zone->next;
			continue;
		}

		if (!zone_type_printed) {
			ft_printf("%s : %p\n", type, (void *)zone);
			
			unsigned int utilization = 0;
			if (stats.total_bytes > 0)
				utilization = (unsigned int)((stats.used_bytes * 100) / stats.total_bytes);
			
			ft_printf("  Statistics: %u chunks used, %u chunks free\n", 
				stats.used_chunks, stats.free_chunks);
			ft_printf("  Memory: %u bytes used / %u bytes total (%u%% utilization)\n",
				(unsigned int)stats.used_bytes, (unsigned int)stats.total_bytes, utilization);
			ft_printf("  Allocations:\n");
			
			zone_type_printed = 1;
		}

		total += print_zone_chunks_ex(zone, show_hexdump);

		zone = zone->next;
	}

	return total;
}

void show_alloc_mem_ex(void) {
	size_t total = 0;
	pthread_t thread_id = pthread_self();
	t_env_flags *flags = env_flags_singleton();
	int show_hexdump = (env_get_alloc_logs(flags) > 0);

	ft_printf("=== Extended Memory Report (Thread %p) ===\n", (void *)thread_id);

	total += show_zone_list_ex(g_thread_zones.tiny, "TINY", show_hexdump);
	total += show_zone_list_ex(g_thread_zones.small, "SMALL", show_hexdump);
	total += show_zone_list_ex(g_thread_zones.large, "LARGE", show_hexdump);

	ft_printf("Total : %u bytes\n", (unsigned int)total);
}
