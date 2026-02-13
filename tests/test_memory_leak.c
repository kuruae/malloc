/* Memory leak detection test */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void show_alloc_mem(void);

int main(void) {
    printf("=== Memory Leak Test ===\n\n");
    
    printf("Initial state (should be empty):\n");
    show_alloc_mem();
    printf("\n");
    
    // Test 1: Allocate and free properly - no leak
    printf("Test 1: Proper alloc/free cycle\n");
    {
        void *ptrs[100];
        for (int i = 0; i < 100; i++) {
            ptrs[i] = malloc(64);
        }
        printf("After 100 allocations:\n");
        show_alloc_mem();
        
        for (int i = 0; i < 100; i++) {
            free(ptrs[i]);
        }
        printf("After freeing all:\n");
        show_alloc_mem();
    }
    printf("\n");
    
    // Test 2: Intentional leak (for demonstration)
    printf("Test 2: Intentional memory leak (10 allocations not freed)\n");
    {
        for (int i = 0; i < 10; i++) {
            void *leak = malloc(100);
            (void)leak;  // Intentionally not freed
        }
        printf("After leaking 10 allocations:\n");
        show_alloc_mem();
    }
    printf("\n");
    
    // Test 3: Complex pattern
    printf("Test 3: Complex alloc/realloc/free pattern\n");
    {
        void *p1 = malloc(50);
        void *p2 = malloc(500);
        void *p3 = malloc(5000);
        
        printf("After 3 different zone allocations:\n");
        show_alloc_mem();
        
        p1 = realloc(p1, 100);
        p2 = realloc(p2, 1000);
        p3 = realloc(p3, 2500);
        
        printf("After reallocs:\n");
        show_alloc_mem();
        
        free(p1);
        free(p2);
        free(p3);
        
        printf("After freeing all (should show only the 10 leaked):\n");
        show_alloc_mem();
    }
    printf("\n");
    
    printf("=== Memory Leak Test Complete ===\n");
    printf("Note: The 10 intentionally leaked allocations should appear above.\n");
    
    return 0;
}
