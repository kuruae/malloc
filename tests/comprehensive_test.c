/* Comprehensive malloc test suite */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <setjmp.h>

#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define NC      "\033[0m"

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) \
    printf(YELLOW "Testing: %s" NC "\n", name);

#define ASSERT(cond, msg) \
    if (!(cond)) { \
        printf(RED "  FAIL: %s" NC "\n", msg); \
        tests_failed++; \
        return; \
    } else { \
        printf(GREEN "  PASS: %s" NC "\n", msg); \
        tests_passed++; \
    }

#define ASSERT_NOT_NULL(ptr, msg) ASSERT((ptr) != NULL, msg)
#define ASSERT_NULL(ptr, msg) ASSERT((ptr) == NULL, msg)
#define ASSERT_EQ(a, b, msg) ASSERT((a) == (b), msg)

/* ===================== BASIC TESTS ===================== */

void test_malloc_basic(void) {
    TEST("Basic malloc");
    
    void *ptr = malloc(100);
    ASSERT_NOT_NULL(ptr, "malloc(100) should return non-NULL");
    
    memset(ptr, 'A', 100);
    ASSERT(((char*)ptr)[0] == 'A', "Memory should be writable");
    
    free(ptr);
    ASSERT(1, "free() should not crash");
}

void test_malloc_zero(void) {
    TEST("malloc(0)");
    
    void *ptr = malloc(0);
    // Standard says malloc(0) may return NULL or a unique pointer
    // Either behavior is acceptable
    if (ptr != NULL) {
        free(ptr);
        ASSERT(1, "malloc(0) returned non-NULL, free should work");
    } else {
        ASSERT(1, "malloc(0) returned NULL (acceptable)");
    }
}

void test_free_null(void) {
    TEST("free(NULL)");
    
    free(NULL);
    ASSERT(1, "free(NULL) should not crash");
}

void test_realloc_basic(void) {
    TEST("Basic realloc");
    
    char *ptr = malloc(50);
    ASSERT_NOT_NULL(ptr, "malloc(50) should succeed");
    
    memcpy(ptr, "Hello, World!", 14);
    
    ptr = realloc(ptr, 100);
    ASSERT_NOT_NULL(ptr, "realloc to 100 should succeed");
    ASSERT(strcmp(ptr, "Hello, World!") == 0, "Data should be preserved");
    
    ptr = realloc(ptr, 25);
    ASSERT_NOT_NULL(ptr, "realloc to 25 should succeed");
    ASSERT(strncmp(ptr, "Hello, World!", 13) == 0, "Data should be preserved on shrink");
    
    free(ptr);
}

void test_realloc_null(void) {
    TEST("realloc(NULL, size)");
    
    void *ptr = realloc(NULL, 100);
    ASSERT_NOT_NULL(ptr, "realloc(NULL, 100) should act as malloc");
    
    free(ptr);
}

void test_realloc_zero(void) {
    TEST("realloc(ptr, 0)");
    
    void *ptr = malloc(100);
    ASSERT_NOT_NULL(ptr, "malloc(100) should succeed");
    
    void *result = realloc(ptr, 0);
    // Standard says realloc(ptr, 0) may return NULL (and free ptr) or a unique pointer
    if (result == NULL) {
        ASSERT(1, "realloc(ptr, 0) returned NULL (acts as free)");
    } else {
        free(result);
        ASSERT(1, "realloc(ptr, 0) returned non-NULL");
    }
}

void test_calloc_basic(void) {
    TEST("Basic calloc");
    
    int *arr = calloc(10, sizeof(int));
    ASSERT_NOT_NULL(arr, "calloc(10, sizeof(int)) should succeed");
    
    int all_zero = 1;
    for (int i = 0; i < 10; i++) {
        if (arr[i] != 0) all_zero = 0;
    }
    ASSERT(all_zero, "calloc should zero-initialize memory");
    
    free(arr);
}

/* ===================== ZONE TESTS ===================== */

void test_tiny_allocations(void) {
    TEST("TINY zone allocations");
    
    // TINY is typically <= 128 bytes
    void *ptrs[200];
    int i;
    
    for (i = 0; i < 200; i++) {
        ptrs[i] = malloc(64);
        if (ptrs[i] == NULL) break;
        memset(ptrs[i], i & 0xFF, 64);
    }
    ASSERT(i == 200, "Should be able to allocate 200 TINY chunks");
    
    // Verify data integrity
    int data_ok = 1;
    for (i = 0; i < 200; i++) {
        if (((unsigned char*)ptrs[i])[0] != (i & 0xFF)) {
            data_ok = 0;
            break;
        }
    }
    ASSERT(data_ok, "Data integrity should be maintained");
    
    for (i = 0; i < 200; i++) {
        free(ptrs[i]);
    }
    ASSERT(1, "All TINY chunks should be freed");
}

void test_small_allocations(void) {
    TEST("SMALL zone allocations");
    
    // SMALL is typically 129-1024 bytes
    void *ptrs[200];
    int i;
    
    for (i = 0; i < 200; i++) {
        ptrs[i] = malloc(512);
        if (ptrs[i] == NULL) break;
        memset(ptrs[i], i & 0xFF, 512);
    }
    ASSERT(i == 200, "Should be able to allocate 200 SMALL chunks");
    
    for (i = 0; i < 200; i++) {
        free(ptrs[i]);
    }
    ASSERT(1, "All SMALL chunks should be freed");
}

void test_large_allocations(void) {
    TEST("LARGE zone allocations");
    
    void *ptrs[50];
    int i;
    
    for (i = 0; i < 50; i++) {
        ptrs[i] = malloc(8192);
        if (ptrs[i] == NULL) break;
        memset(ptrs[i], i & 0xFF, 8192);
    }
    ASSERT(i == 50, "Should be able to allocate 50 LARGE chunks");
    
    for (i = 0; i < 50; i++) {
        free(ptrs[i]);
    }
    ASSERT(1, "All LARGE chunks should be freed");
}

void test_zone_boundaries(void) {
    TEST("Zone boundary sizes");
    
    // Test around typical TINY/SMALL boundary (128 bytes)
    void *tiny = malloc(128);
    void *small = malloc(129);
    void *large = malloc(1025);
    
    ASSERT_NOT_NULL(tiny, "malloc(128) should succeed");
    ASSERT_NOT_NULL(small, "malloc(129) should succeed");
    ASSERT_NOT_NULL(large, "malloc(1025) should succeed");
    
    free(tiny);
    free(small);
    free(large);
}

/* ===================== ALIGNMENT TESTS ===================== */

void test_alignment(void) {
    TEST("Memory alignment");
    
    // Test that returned pointers are properly aligned
    int aligned = 1;
    
    for (int size = 1; size <= 1024; size++) {
        void *ptr = malloc(size);
        if (ptr == NULL) {
            aligned = 0;
            break;
        }
        
        // Should be at least 16-byte aligned on most systems
        if ((uintptr_t)ptr % 16 != 0) {
            aligned = 0;
            printf("  Misaligned at size %d: %p\n", size, ptr);
        }
        free(ptr);
        
        if (!aligned) break;
    }
    ASSERT(aligned, "All allocations should be 16-byte aligned");
}

/* ===================== EDGE CASES ===================== */

void test_very_large_allocation(void) {
    TEST("Very large allocation");
    
    // Try to allocate 100MB
    void *ptr = malloc(100 * 1024 * 1024);
    if (ptr != NULL) {
        memset(ptr, 0, 100 * 1024 * 1024);
        free(ptr);
        ASSERT(1, "100MB allocation succeeded");
    } else {
        ASSERT(1, "100MB allocation failed (may be expected)");
    }
}

void test_many_small_allocations(void) {
    TEST("Many small allocations (stress test)");
    
    void *ptrs[10000];
    int i;
    
    for (i = 0; i < 10000; i++) {
        ptrs[i] = malloc(1 + (i % 64));
        if (ptrs[i] == NULL) break;
    }
    
    int allocated = i;
    ASSERT(allocated == 10000, "Should allocate 10000 small chunks");
    
    for (i = 0; i < allocated; i++) {
        free(ptrs[i]);
    }
    ASSERT(1, "All chunks freed");
}

void test_alternating_sizes(void) {
    TEST("Alternating size allocations");
    
    void *ptrs[100];
    
    for (int i = 0; i < 100; i++) {
        // Alternate between TINY, SMALL, and LARGE
        size_t size;
        switch (i % 3) {
            case 0: size = 32; break;    // TINY
            case 1: size = 500; break;   // SMALL
            case 2: size = 5000; break;  // LARGE
        }
        ptrs[i] = malloc(size);
        ASSERT_NOT_NULL(ptrs[i], "Allocation should succeed");
        memset(ptrs[i], i, size);
    }
    
    for (int i = 0; i < 100; i++) {
        free(ptrs[i]);
    }
    ASSERT(1, "All alternating allocations freed");
}

void test_realloc_growth_shrink(void) {
    TEST("realloc growth and shrink patterns");
    
    char *ptr = malloc(10);
    ASSERT_NOT_NULL(ptr, "Initial malloc");
    strcpy(ptr, "test");
    
    // Grow within TINY
    ptr = realloc(ptr, 100);
    ASSERT_NOT_NULL(ptr, "Grow to 100");
    ASSERT(strcmp(ptr, "test") == 0, "Data preserved after grow");
    
    // Grow to SMALL
    ptr = realloc(ptr, 500);
    ASSERT_NOT_NULL(ptr, "Grow to 500 (SMALL)");
    ASSERT(strcmp(ptr, "test") == 0, "Data preserved crossing to SMALL");
    
    // Grow to LARGE
    ptr = realloc(ptr, 5000);
    ASSERT_NOT_NULL(ptr, "Grow to 5000 (LARGE)");
    ASSERT(strcmp(ptr, "test") == 0, "Data preserved crossing to LARGE");
    
    // Shrink back to SMALL
    ptr = realloc(ptr, 500);
    ASSERT_NOT_NULL(ptr, "Shrink to 500");
    ASSERT(strcmp(ptr, "test") == 0, "Data preserved after shrink");
    
    // Shrink to TINY
    ptr = realloc(ptr, 50);
    ASSERT_NOT_NULL(ptr, "Shrink to 50 (TINY)");
    ASSERT(strcmp(ptr, "test") == 0, "Data preserved crossing to TINY");
    
    free(ptr);
}

void test_size_1_allocation(void) {
    TEST("Size 1 allocation");
    
    char *ptr = malloc(1);
    ASSERT_NOT_NULL(ptr, "malloc(1) should succeed");
    
    ptr[0] = 'A';
    ASSERT(ptr[0] == 'A', "Single byte should be writable");
    
    free(ptr);
}

void test_exact_page_size(void) {
    TEST("Exact page size allocation");
    
    long page_size = sysconf(_SC_PAGESIZE);
    
    void *ptr = malloc(page_size);
    ASSERT_NOT_NULL(ptr, "malloc(page_size) should succeed");
    memset(ptr, 0, page_size);
    free(ptr);
    
    ptr = malloc(page_size - 1);
    ASSERT_NOT_NULL(ptr, "malloc(page_size - 1) should succeed");
    memset(ptr, 0, page_size - 1);
    free(ptr);
    
    ptr = malloc(page_size + 1);
    ASSERT_NOT_NULL(ptr, "malloc(page_size + 1) should succeed");
    memset(ptr, 0, page_size + 1);
    free(ptr);
}

/* ===================== FREE PATTERNS ===================== */

void test_reverse_free(void) {
    TEST("Reverse order free");
    
    void *ptrs[100];
    for (int i = 0; i < 100; i++) {
        ptrs[i] = malloc(100);
        ASSERT_NOT_NULL(ptrs[i], "Allocation");
    }
    
    for (int i = 99; i >= 0; i--) {
        free(ptrs[i]);
    }
    ASSERT(1, "Reverse free completed");
}

void test_random_free_pattern(void) {
    TEST("Random free pattern");
    
    void *ptrs[100];
    for (int i = 0; i < 100; i++) {
        ptrs[i] = malloc(100);
    }
    
    // Free even indices first
    for (int i = 0; i < 100; i += 2) {
        free(ptrs[i]);
        ptrs[i] = NULL;
    }
    
    // Allocate new ones
    for (int i = 0; i < 100; i += 2) {
        ptrs[i] = malloc(50);
    }
    
    // Free all
    for (int i = 0; i < 100; i++) {
        free(ptrs[i]);
    }
    ASSERT(1, "Random free pattern completed");
}

void test_interleaved_malloc_free(void) {
    TEST("Interleaved malloc/free");
    
    for (int i = 0; i < 1000; i++) {
        void *ptr = malloc(1 + (i % 1000));
        if (ptr) {
            memset(ptr, i & 0xFF, 1 + (i % 1000));
            free(ptr);
        }
    }
    ASSERT(1, "Interleaved malloc/free completed");
}

/* ===================== COALESCING TESTS ===================== */

void test_coalescing(void) {
    TEST("Chunk coalescing");
    
    // Allocate three chunks
    void *a = malloc(100);
    void *b = malloc(100);
    void *c = malloc(100);
    
    ASSERT_NOT_NULL(a, "Allocate a");
    ASSERT_NOT_NULL(b, "Allocate b");
    ASSERT_NOT_NULL(c, "Allocate c");
    
    // Free middle chunk
    free(b);
    
    // Free first chunk (should coalesce with b)
    free(a);
    
    // Free last chunk (should coalesce with a+b)
    free(c);
    
    // Now allocate a large chunk that should fit in coalesced space
    void *big = malloc(250);
    ASSERT_NOT_NULL(big, "Large allocation after coalescing");
    
    free(big);
}

/* ===================== MEMORY CORRUPTION DETECTION ===================== */

void test_write_after_allocation(void) {
    TEST("Write to entire allocated region");
    
    for (int size = 1; size <= 8192; size *= 2) {
        char *ptr = malloc(size);
        ASSERT_NOT_NULL(ptr, "Allocation");
        
        // Write to every byte
        for (int i = 0; i < size; i++) {
            ptr[i] = (char)(i & 0xFF);
        }
        
        // Verify
        int ok = 1;
        for (int i = 0; i < size; i++) {
            if (ptr[i] != (char)(i & 0xFF)) {
                ok = 0;
                break;
            }
        }
        ASSERT(ok, "Data integrity check");
        
        free(ptr);
    }
}

/* ===================== THREAD SAFETY TESTS ===================== */

#define NUM_THREADS 10
#define ALLOCS_PER_THREAD 1000

void *thread_func(void *arg) {
    int id = *(int*)arg;
    void *ptrs[ALLOCS_PER_THREAD];
    
    for (int i = 0; i < ALLOCS_PER_THREAD; i++) {
        size_t size = 1 + ((id * i) % 1024);
        ptrs[i] = malloc(size);
        if (ptrs[i]) {
            memset(ptrs[i], id, size);
        }
    }
    
    for (int i = 0; i < ALLOCS_PER_THREAD; i++) {
        free(ptrs[i]);
    }
    
    return NULL;
}

void test_multithreaded(void) {
    TEST("Multi-threaded allocation");
    
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    ASSERT(1, "Multi-threaded test completed without crash");
}

/* ===================== OVERFLOW TESTS ===================== */

void test_size_overflow(void) {
    TEST("Size overflow protection");
    
    // Try to allocate SIZE_MAX
    void *ptr = malloc(SIZE_MAX);
    ASSERT_NULL(ptr, "malloc(SIZE_MAX) should return NULL");
    
    // Try to allocate SIZE_MAX - small value
    ptr = malloc(SIZE_MAX - 100);
    ASSERT_NULL(ptr, "malloc(SIZE_MAX - 100) should return NULL");
    
    // calloc overflow: nmemb * size overflows
    ptr = calloc(SIZE_MAX, 2);
    ASSERT_NULL(ptr, "calloc(SIZE_MAX, 2) should return NULL");
    
    ptr = calloc(SIZE_MAX / 2 + 1, 2);
    ASSERT_NULL(ptr, "calloc overflow should return NULL");
}

/* ===================== SPECIAL PATTERNS ===================== */

void test_string_operations(void) {
    TEST("String operations");
    
    char *s1 = malloc(100);
    ASSERT_NOT_NULL(s1, "Allocate s1");
    strcpy(s1, "Hello, World!");
    
    char *s2 = malloc(strlen(s1) + 1);
    ASSERT_NOT_NULL(s2, "Allocate s2");
    strcpy(s2, s1);
    
    ASSERT(strcmp(s1, s2) == 0, "Strings should match");
    
    char *s3 = realloc(s1, 200);
    ASSERT_NOT_NULL(s3, "Realloc s1");
    strcat(s3, " Extended!");
    ASSERT(strcmp(s3, "Hello, World! Extended!") == 0, "Concatenation should work");
    
    free(s2);
    free(s3);
}

void test_struct_allocation(void) {
    TEST("Structure allocation");
    
    typedef struct {
        int x;
        double y;
        char name[64];
    } TestStruct;
    
    TestStruct *s = malloc(sizeof(TestStruct));
    ASSERT_NOT_NULL(s, "Allocate struct");
    
    s->x = 42;
    s->y = 3.14159;
    strcpy(s->name, "Test");
    
    ASSERT(s->x == 42, "Int field should be preserved");
    ASSERT(s->y > 3.14 && s->y < 3.15, "Double field should be preserved");
    ASSERT(strcmp(s->name, "Test") == 0, "String field should be preserved");
    
    free(s);
}

void test_array_of_pointers(void) {
    TEST("Array of pointers");
    
    char **arr = malloc(100 * sizeof(char*));
    ASSERT_NOT_NULL(arr, "Allocate pointer array");
    
    for (int i = 0; i < 100; i++) {
        arr[i] = malloc(32);
        if (arr[i]) {
            sprintf(arr[i], "String %d", i);
        }
    }
    
    int ok = 1;
    char expected[32];
    for (int i = 0; i < 100; i++) {
        sprintf(expected, "String %d", i);
        if (arr[i] && strcmp(arr[i], expected) != 0) {
            ok = 0;
            break;
        }
    }
    ASSERT(ok, "All strings should be correct");
    
    for (int i = 0; i < 100; i++) {
        free(arr[i]);
    }
    free(arr);
}

/* ===================== STRESS TESTS ===================== */

void test_rapid_alloc_free(void) {
    TEST("Rapid alloc/free cycles");
    
    for (int cycle = 0; cycle < 100; cycle++) {
        void *ptrs[100];
        for (int i = 0; i < 100; i++) {
            ptrs[i] = malloc(64);
        }
        for (int i = 0; i < 100; i++) {
            free(ptrs[i]);
        }
    }
    ASSERT(1, "Rapid cycles completed");
}

void test_growing_allocations(void) {
    TEST("Growing allocation sizes");
    
    void *ptrs[20];
    size_t size = 1;
    
    for (int i = 0; i < 20; i++) {
        ptrs[i] = malloc(size);
        if (ptrs[i]) {
            memset(ptrs[i], i, size);
        }
        size *= 2;
    }
    
    for (int i = 0; i < 20; i++) {
        free(ptrs[i]);
    }
    ASSERT(1, "Growing allocations completed");
}

/* ===================== REALLOC SPECIAL CASES ===================== */

void test_realloc_same_size(void) {
    TEST("realloc to same size");
    
    char *ptr = malloc(100);
    ASSERT_NOT_NULL(ptr, "Initial malloc");
    strcpy(ptr, "test data");
    
    char *new_ptr = realloc(ptr, 100);
    ASSERT_NOT_NULL(new_ptr, "Realloc same size");
    ASSERT(strcmp(new_ptr, "test data") == 0, "Data should be preserved");
    
    free(new_ptr);
}

void test_realloc_slightly_larger(void) {
    TEST("realloc to slightly larger");
    
    char *ptr = malloc(100);
    ASSERT_NOT_NULL(ptr, "Initial malloc");
    memset(ptr, 'A', 100);
    
    ptr = realloc(ptr, 101);
    ASSERT_NOT_NULL(ptr, "Realloc +1 byte");
    ASSERT(ptr[0] == 'A' && ptr[99] == 'A', "Data should be preserved");
    
    free(ptr);
}

void test_realloc_much_larger(void) {
    TEST("realloc to much larger (zone change)");
    
    char *ptr = malloc(32);  // TINY
    ASSERT_NOT_NULL(ptr, "Initial TINY malloc");
    strcpy(ptr, "small data");
    
    ptr = realloc(ptr, 10000);  // LARGE
    ASSERT_NOT_NULL(ptr, "Realloc to LARGE");
    ASSERT(strcmp(ptr, "small data") == 0, "Data preserved across zone change");
    
    free(ptr);
}

/* ===================== MAIN ===================== */

int main(void) {
    printf(BLUE "\n=================================================\n");
    printf("       COMPREHENSIVE MALLOC TEST SUITE\n");
    printf("=================================================\n\n" NC);
    
    // Basic tests
    test_malloc_basic();
    test_malloc_zero();
    test_free_null();
    test_realloc_basic();
    test_realloc_null();
    test_realloc_zero();
    test_calloc_basic();
    
    // Zone tests
    test_tiny_allocations();
    test_small_allocations();
    test_large_allocations();
    test_zone_boundaries();
    
    // Alignment tests
    test_alignment();
    
    // Edge cases
    test_very_large_allocation();
    test_many_small_allocations();
    test_alternating_sizes();
    test_realloc_growth_shrink();
    test_size_1_allocation();
    test_exact_page_size();
    
    // Free patterns
    test_reverse_free();
    test_random_free_pattern();
    test_interleaved_malloc_free();
    
    // Coalescing
    test_coalescing();
    
    // Memory integrity
    test_write_after_allocation();
    
    // Thread safety
    test_multithreaded();
    
    // Overflow protection
    test_size_overflow();
    
    // Special patterns
    test_string_operations();
    test_struct_allocation();
    test_array_of_pointers();
    
    // Stress tests
    test_rapid_alloc_free();
    test_growing_allocations();
    
    // Realloc special cases
    test_realloc_same_size();
    test_realloc_slightly_larger();
    test_realloc_much_larger();
    
    printf(BLUE "\n=================================================\n");
    printf("                    SUMMARY\n");
    printf("=================================================\n" NC);
    printf("Passed: " GREEN "%d" NC "\n", tests_passed);
    printf("Failed: " RED "%d" NC "\n", tests_failed);
    printf("Total:  %d\n\n", tests_passed + tests_failed);
    
    if (tests_failed == 0) {
        printf(GREEN "ALL TESTS PASSED!\n" NC);
        return 0;
    } else {
        printf(RED "SOME TESTS FAILED!\n" NC);
        return 1;
    }
}
