# malloc

Custom malloc implementation using memory zones and mmap. (school project from 42)

## What this is

A dynamic memory allocator that replaces the standard `malloc()`, `free()`, and `realloc()`. Uses a zone-based approach to categorize allocations by size (tiny, small, large) for better memory management.

Currently only large allocations work. Tiny and small zones are still TODO.

## How it works

Memory is organized into zones. Each zone is an mmap'd region that contains:
- A zone header tracking metadata (size, type, next zone in list)
- One or more chunks of allocated memory
- Each chunk has a header (size, free status, next chunk)

Three zone types:
- **TINY**: allocations ≤ page_size/32
- **SMALL**: allocations ≤ page_size/4  
- **LARGE**: allocations > page_size/4 (each gets its own dedicated zone)

The allocator maintains separate linked lists for each zone type.

## Implementation details

### Size calculations
```
TINY_MAX  = page_size / 32    (typically 128 bytes on 4KB pages)
SMALL_MAX = page_size / 4     (typically 1024 bytes)
LARGE     = anything larger
```

Zone sizes for tiny/small are calculated to hold at least 100 allocations:
```
zone_size = (max_type_size + headers) * MIN_ALLOC_COUNT
```
Then rounded up to page boundary for efficient mmap.

### Memory layout
```
[zone_header][chunk_header][user_data][chunk_header][user_data]...
```

Pointers returned to users point to the data section. The chunk header sits right before it, so `free()` can find metadata by subtracting `sizeof(t_chunk_header)`.

### Alignment
All structures use `alignof(max_align_t)` to ensure proper alignment for any type.

## Build

```bash
make        # builds libft_malloc_<HOSTTYPE>.so and symlinks to libft_malloc.so
make clean  # removes object files
make fclean # removes everything
make re     # rebuild from scratch
```

The library is compiled with `-fPIC` and linked as a shared object. It depends on libft (built automatically).

### Usage

Preload the library to override system malloc:
```bash
export LD_LIBRARY_PATH=.
export LD_PRELOAD=./libft_malloc.so
./your_program
```

Or link directly when compiling:
```bash
gcc your_program.c -L. -lft_malloc
```
