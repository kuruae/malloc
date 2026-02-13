/* Test show_alloc_mem specifically */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void show_alloc_mem(void);

int main(void) {
    printf("=== Testing show_alloc_mem ===\n\n");
    
    printf("Before any allocations:\n");
    show_alloc_mem();
    printf("\n");
    
    printf("After malloc(32) (TINY):\n");
    void *tiny = malloc(32);
    printf("  tiny ptr = %p\n", tiny);
    show_alloc_mem();
    printf("\n");
    
    printf("After malloc(500) (SMALL):\n");
    void *small = malloc(500);
    printf("  small ptr = %p\n", small);
    show_alloc_mem();
    printf("\n");
    
    printf("After malloc(5000) (LARGE):\n");
    void *large = malloc(5000);
    printf("  large ptr = %p\n", large);
    show_alloc_mem();
    printf("\n");
    
    printf("After freeing tiny:\n");
    free(tiny);
    show_alloc_mem();
    printf("\n");
    
    printf("After freeing all:\n");
    free(small);
    free(large);
    show_alloc_mem();
    printf("\n");
    
    printf("=== Done ===\n");
    return 0;
}
