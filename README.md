# malloc

Custom malloc implementation using memory zones and mmap. (school project from 42)

## What this is

A dynamic memory allocator that replaces the standard `malloc()`, `free()`, and `realloc()`. Uses a zone-based approach to categorize allocations by size (tiny, small, large) for better memory management.


## How it works

Memory is organized into zones. Each zone is an mmap'd region that contains:
- A zone header tracking metadata (size, type, next zone in list)
- A break pointer marking where unallocated space begins
- One or more chunks of allocated memory (carved on-demand)
- Each chunk has a header (size, free status, next chunk pointer)

Three zone types:
- **TINY**: allocations ≤ page_size/32 (shared zones with multiple allocations)
- **SMALL**: allocations ≤ page_size/4 (shared zones with multiple allocations)
- **LARGE**: allocations > page_size/4 (each gets its own dedicated zone)

The allocator maintains separate linked lists for each zone type. When allocating, it searches existing zones for free chunks or available space. If none is found, a new zone is created.

## TODO:

- fragmentation handling
- thread safety (mutexes)
- ~~show_alloc_mem() function for debugging~~
- show_alloc_mem_ex() function for more advanced debugging

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
Zone (tiny/small):
[zone_header][break_ptr→][chunk_header][user_data][chunk_header][user_data]...[free space]

Zone (large):
[zone_header][chunk_header][user_data]
```

Chunks are carved on-demand from unallocated space as allocations are requested. When a chunk is freed (tiny/small), it's marked as available for reuse. Large allocations are immediately unmapped when freed.

Pointers returned to users point to the data section. The chunk header sits right before it, so `free()` can find metadata by subtracting `sizeof(t_chunk_header)`.

### Alignment
All structures use `alignof(max_align_t)` to ensure proper alignment for any type.

## Build

```bash
make        # builds libft_malloc_<HOSTTYPE>.so and symlinks to libft_malloc.so
make clean  
make fclean 
make re     
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

## Testing

Run individual tests:
```bash
make test TESTFILE=tests/test_tiny.c
make test TESTFILE=tests/test_small.c
make test TESTFILE=tests/test_free.c
make test TESTFILE=tests/test_edge_cases.c
make test TESTFILE=tests/test_failures.c
```

Run all tests at once:
```bash
./tests/run_all_tests.sh
```
