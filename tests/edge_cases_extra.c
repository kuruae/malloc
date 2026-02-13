/* Additional edge case tests */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/resource.h>

#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define NC      "\033[0m"

/* Test that show_alloc_mem doesn't crash */
extern void show_alloc_mem(void);
extern void show_alloc_mem_ex(void);

void test_show_alloc_mem_basic(void) {
    printf(YELLOW "Testing show_alloc_mem()...\n" NC);
    
    void *p1 = malloc(32);
    void *p2 = malloc(200);
    void *p3 = malloc(5000);
    
    printf("Allocated: tiny=%p, small=%p, large=%p\n", p1, p2, p3);
    
    printf("\n--- show_alloc_mem() output ---\n");
    show_alloc_mem();
    printf("--- end output ---\n\n");
    
    free(p1);
    free(p2);
    free(p3);
    
    printf(GREEN "PASS: show_alloc_mem() completed without crash\n" NC);
}

/* Test realloc preserves data for various patterns */
void test_realloc_data_preservation(void) {
    printf(YELLOW "Testing realloc data preservation patterns...\n" NC);
    
    // Create a pattern
    char *ptr = malloc(1000);
    for (int i = 0; i < 1000; i++) {
        ptr[i] = 'A' + (i % 26);
    }
    
    // Grow significantly 
    ptr = realloc(ptr, 10000);
    int ok = 1;
    for (int i = 0; i < 1000; i++) {
        if (ptr[i] != 'A' + (i % 26)) {
            ok = 0;
            printf(RED "  Data corruption at index %d\n" NC, i);
            break;
        }
    }
    
    // Shrink back
    ptr = realloc(ptr, 500);
    for (int i = 0; i < 500; i++) {
        if (ptr[i] != 'A' + (i % 26)) {
            ok = 0;
            printf(RED "  Data corruption after shrink at index %d\n" NC, i);
            break;
        }
    }
    
    free(ptr);
    
    if (ok) {
        printf(GREEN "PASS: Realloc data preservation\n" NC);
    } else {
        printf(RED "FAIL: Realloc data preservation\n" NC);
    }
}

/* Test that memory returned is usable up to size boundary */
void test_memory_bounds(void) {
    printf(YELLOW "Testing memory bounds usage...\n" NC);
    
    size_t sizes[] = {1, 15, 16, 17, 31, 32, 33, 63, 64, 65, 
                      127, 128, 129, 255, 256, 512, 1023, 1024, 1025,
                      2048, 4096, 8192, 16384};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int ok = 1;
    
    for (int i = 0; i < num_sizes && ok; i++) {
        char *ptr = malloc(sizes[i]);
        if (!ptr) {
            printf(RED "  malloc(%zu) failed\n" NC, sizes[i]);
            ok = 0;
            break;
        }
        
        // Write to every byte
        memset(ptr, 0xAA, sizes[i]);
        
        // Verify
        for (size_t j = 0; j < sizes[i]; j++) {
            if ((unsigned char)ptr[j] != 0xAA) {
                printf(RED "  Corruption at size %zu, offset %zu\n" NC, sizes[i], j);
                ok = 0;
                break;
            }
        }
        
        free(ptr);
    }
    
    if (ok) {
        printf(GREEN "PASS: Memory bounds\n" NC);
    } else {
        printf(RED "FAIL: Memory bounds\n" NC);
    }
}

/* Test allocating many different sizes in sequence */
void test_mixed_sizes_sequence(void) {
    printf(YELLOW "Testing mixed sizes allocation sequence...\n" NC);
    
    void *ptrs[1000];
    int ok = 1;
    
    // Allocate many different sizes
    for (int i = 0; i < 1000; i++) {
        size_t size = 1 + (i * 17) % 5000;  // Various sizes
        ptrs[i] = malloc(size);
        if (!ptrs[i]) {
            printf(RED "  malloc(%zu) failed at i=%d\n" NC, size, i);
            ok = 0;
            break;
        }
        // Touch the memory
        memset(ptrs[i], i & 0xFF, size);
    }
    
    // Free in random order (using a simple pattern)
    for (int i = 0; i < 1000; i += 3) {
        free(ptrs[i]);
        ptrs[i] = NULL;
    }
    for (int i = 1; i < 1000; i += 3) {
        free(ptrs[i]);
        ptrs[i] = NULL;
    }
    for (int i = 2; i < 1000; i += 3) {
        free(ptrs[i]);
        ptrs[i] = NULL;
    }
    
    if (ok) {
        printf(GREEN "PASS: Mixed sizes sequence\n" NC);
    } else {
        printf(RED "FAIL: Mixed sizes sequence\n" NC);
    }
}

/* Test realloc NULL and zero combinations */
void test_realloc_special_cases(void) {
    printf(YELLOW "Testing realloc special cases...\n" NC);
    int ok = 1;
    
    // realloc(NULL, 0) - implementation defined, should not crash
    void *ptr = realloc(NULL, 0);
    // Either NULL or valid pointer is acceptable
    if (ptr) free(ptr);
    
    // realloc(NULL, size) should be like malloc
    ptr = realloc(NULL, 100);
    if (!ptr) {
        printf(RED "  realloc(NULL, 100) returned NULL\n" NC);
        ok = 0;
    } else {
        strcpy(ptr, "test");
        free(ptr);
    }
    
    // realloc(ptr, 0) should free and return NULL (or implementation defined)
    ptr = malloc(100);
    if (ptr) {
        void *result = realloc(ptr, 0);
        if (result) free(result);  // If it returns something, free it
    }
    
    if (ok) {
        printf(GREEN "PASS: Realloc special cases\n" NC);
    } else {
        printf(RED "FAIL: Realloc special cases\n" NC);
    }
}

/* Test calloc edge cases */
void test_calloc_edge_cases(void) {
    printf(YELLOW "Testing calloc edge cases...\n" NC);
    int ok = 1;
    
    // calloc(0, size) - may return NULL or unique pointer
    void *ptr = calloc(0, 100);
    if (ptr) free(ptr);
    
    // calloc(nmemb, 0) - may return NULL or unique pointer
    ptr = calloc(100, 0);
    if (ptr) free(ptr);
    
    // calloc(0, 0) - may return NULL or unique pointer
    ptr = calloc(0, 0);
    if (ptr) free(ptr);
    
    // Normal calloc should zero-init
    int *arr = calloc(100, sizeof(int));
    if (!arr) {
        printf(RED "  calloc(100, sizeof(int)) failed\n" NC);
        ok = 0;
    } else {
        for (int i = 0; i < 100; i++) {
            if (arr[i] != 0) {
                printf(RED "  calloc memory not zero at index %d\n" NC, i);
                ok = 0;
                break;
            }
        }
        free(arr);
    }
    
    if (ok) {
        printf(GREEN "PASS: Calloc edge cases\n" NC);
    } else {
        printf(RED "FAIL: Calloc edge cases\n" NC);
    }
}

/* Test pointer alignment for various types */
void test_alignment_requirements(void) {
    printf(YELLOW "Testing alignment requirements...\n" NC);
    int ok = 1;
    
    // Test alignment for various type sizes
    for (int i = 0; i < 100; i++) {
        // Test that we can safely use the memory for any type
        void *ptr = malloc(sizeof(long double) * 10);
        if (!ptr) {
            ok = 0;
            break;
        }
        
        // Should be aligned for long double (typically 16 bytes on most systems)
        if ((uintptr_t)ptr % _Alignof(long double) != 0) {
            printf(RED "  Pointer %p not aligned for long double\n" NC, ptr);
            ok = 0;
        }
        
        // Should be aligned for long long
        if ((uintptr_t)ptr % _Alignof(long long) != 0) {
            printf(RED "  Pointer %p not aligned for long long\n" NC, ptr);
            ok = 0;
        }
        
        // Should be aligned for double
        if ((uintptr_t)ptr % _Alignof(double) != 0) {
            printf(RED "  Pointer %p not aligned for double\n" NC, ptr);
            ok = 0;
        }
        
        free(ptr);
        
        if (!ok) break;
    }
    
    if (ok) {
        printf(GREEN "PASS: Alignment requirements\n" NC);
    } else {
        printf(RED "FAIL: Alignment requirements\n" NC);
    }
}

/* Test memory reuse after free */
void test_memory_reuse(void) {
    printf(YELLOW "Testing memory reuse after free...\n" NC);
    
    // Allocate and free repeatedly, should reuse memory
    void *first_ptr = malloc(64);
    free(first_ptr);
    
    void *second_ptr = malloc(64);
    // The addresses might be the same if memory is reused
    printf("  First allocation:  %p\n", first_ptr);
    printf("  Second allocation: %p\n", second_ptr);
    
    free(second_ptr);
    
    printf(GREEN "PASS: Memory reuse test completed\n" NC);
}

/* Test large number of small allocations then large allocations */
void test_fragmentation_resistance(void) {
    printf(YELLOW "Testing fragmentation resistance...\n" NC);
    
    void *small_ptrs[1000];
    
    // Allocate many small chunks
    for (int i = 0; i < 1000; i++) {
        small_ptrs[i] = malloc(16);
    }
    
    // Free every other one (creates fragmentation potential)
    for (int i = 0; i < 1000; i += 2) {
        free(small_ptrs[i]);
        small_ptrs[i] = NULL;
    }
    
    // Try to allocate larger chunks (should still work)
    void *large_ptrs[10];
    int ok = 1;
    for (int i = 0; i < 10; i++) {
        large_ptrs[i] = malloc(8192);
        if (!large_ptrs[i]) {
            printf(RED "  Large allocation %d failed\n" NC, i);
            ok = 0;
            break;
        }
    }
    
    // Cleanup
    for (int i = 0; i < 10; i++) {
        free(large_ptrs[i]);
    }
    for (int i = 1; i < 1000; i += 2) {
        free(small_ptrs[i]);
    }
    
    if (ok) {
        printf(GREEN "PASS: Fragmentation resistance\n" NC);
    } else {
        printf(RED "FAIL: Fragmentation resistance\n" NC);
    }
}

/* Test zone transitions precisely */
void test_zone_transitions(void) {
    printf(YELLOW "Testing zone transitions...\n" NC);
    
    // Get page size to determine zone boundaries
    long page_size = sysconf(_SC_PAGESIZE);
    size_t tiny_max = page_size / 32;   // Typically 128
    size_t small_max = page_size / 4;   // Typically 1024
    
    printf("  Page size: %ld, TINY max: %zu, SMALL max: %zu\n", 
           page_size, tiny_max, small_max);
    
    int ok = 1;
    
    // Test exact boundaries
    void *p1 = malloc(tiny_max);
    void *p2 = malloc(tiny_max + 1);
    void *p3 = malloc(small_max);
    void *p4 = malloc(small_max + 1);
    
    if (!p1 || !p2 || !p3 || !p4) {
        printf(RED "  Boundary allocation failed\n" NC);
        ok = 0;
    } else {
        // Write to ensure they're valid
        memset(p1, 'T', tiny_max);
        memset(p2, 'S', tiny_max + 1);
        memset(p3, 's', small_max);
        memset(p4, 'L', small_max + 1);
    }
    
    free(p1);
    free(p2);
    free(p3);
    free(p4);
    
    if (ok) {
        printf(GREEN "PASS: Zone transitions\n" NC);
    } else {
        printf(RED "FAIL: Zone transitions\n" NC);
    }
}

/* Stress test with rapid malloc/realloc/free */
void test_rapid_operations(void) {
    printf(YELLOW "Testing rapid malloc/realloc/free operations...\n" NC);
    
    void *ptr = NULL;
    
    for (int i = 0; i < 10000; i++) {
        int op = i % 3;
        switch (op) {
            case 0:  // malloc
                if (ptr) free(ptr);
                ptr = malloc(1 + (i % 1000));
                break;
            case 1:  // realloc
                ptr = realloc(ptr, 1 + ((i * 7) % 2000));
                break;
            case 2:  // free and malloc
                free(ptr);
                ptr = malloc(1 + ((i * 13) % 500));
                break;
        }
        
        if (ptr) {
            ((char*)ptr)[0] = 'X';
        }
    }
    
    free(ptr);
    
    printf(GREEN "PASS: Rapid operations\n" NC);
}

/* Test with pthread race conditions */
void *race_thread(void *arg) {
    (void)arg;
    
    for (int i = 0; i < 1000; i++) {
        void *p = malloc(100);
        if (p) {
            memset(p, 'X', 100);
            free(p);
        }
    }
    
    return NULL;
}

void test_thread_race(void) {
    printf(YELLOW "Testing thread race conditions...\n" NC);
    
    pthread_t threads[20];
    
    for (int i = 0; i < 20; i++) {
        pthread_create(&threads[i], NULL, race_thread, NULL);
    }
    
    for (int i = 0; i < 20; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf(GREEN "PASS: Thread race test completed without deadlock/crash\n" NC);
}

int main(void) {
    printf("\n=== ADDITIONAL EDGE CASE TESTS ===\n\n");
    
    test_show_alloc_mem_basic();
    test_realloc_data_preservation();
    test_memory_bounds();
    test_mixed_sizes_sequence();
    test_realloc_special_cases();
    test_calloc_edge_cases();
    test_alignment_requirements();
    test_memory_reuse();
    test_fragmentation_resistance();
    test_zone_transitions();
    test_rapid_operations();
    test_thread_race();
    
    printf("\n=== ALL EDGE CASE TESTS COMPLETED ===\n\n");
    
    return 0;
}
