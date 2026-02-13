/* Test error handling - safe version */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    printf("=== Safe Error Handling Tests ===\n\n");
    
    // Test 1: Double free detection
    printf("Test 1: Double-free detection\n");
    {
        void *ptr = malloc(100);
        printf("  Allocated: %p\n", ptr);
        free(ptr);
        printf("  First free done\n");
        // This should be detected as double-free
        free(ptr);
        printf("  Second free done (should have warned about double-free)\n");
    }
    printf("\n");
    
    // Test 2: NULL is always safe
    printf("Test 2: NULL pointer\n");
    {
        free(NULL);
        free(NULL);
        free(NULL);
        printf("  Multiple NULL frees handled correctly\n");
    }
    printf("\n");
    
    // Test 3: Normal alloc/free cycle
    printf("Test 3: Normal allocation cycle\n");
    {
        for (int i = 0; i < 100; i++) {
            void *ptr = malloc(i + 1);
            if (ptr) {
                memset(ptr, 'X', i + 1);
                free(ptr);
            }
        }
        printf("  100 alloc/free cycles completed\n");
    }
    printf("\n");
    
    // Test 4: Realloc patterns
    printf("Test 4: Realloc patterns\n");
    {
        void *ptr = NULL;
        
        // realloc(NULL, size) acts like malloc
        ptr = realloc(NULL, 100);
        printf("  realloc(NULL, 100) = %p\n", ptr);
        
        if (ptr) {
            strcpy(ptr, "test data");
            
            // Grow
            ptr = realloc(ptr, 200);
            printf("  realloc to 200 = %p, data = '%s'\n", ptr, (char*)ptr);
            
            // Shrink
            ptr = realloc(ptr, 50);
            printf("  realloc to 50 = %p, data = '%s'\n", ptr, (char*)ptr);
            
            // realloc(ptr, 0) acts like free
            void *result = realloc(ptr, 0);
            printf("  realloc(ptr, 0) = %p (should be NULL or freed)\n", result);
            if (result) free(result);
        }
    }
    printf("\n");
    
    // Test 5: Allocation failure handling
    printf("Test 5: Very large allocation\n");
    {
        // Try to allocate SIZE_MAX - should fail
        void *ptr = malloc((size_t)-1);
        if (ptr == NULL) {
            printf("  malloc(SIZE_MAX) correctly returned NULL\n");
        } else {
            printf("  ERROR: malloc(SIZE_MAX) should have returned NULL\n");
            free(ptr);
        }
    }
    printf("\n");
    
    printf("=== All safe error handling tests completed ===\n");
    return 0;
}
