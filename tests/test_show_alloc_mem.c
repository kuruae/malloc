#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare show_alloc_mem to use it
void show_alloc_mem(void);

int main(void) {
	printf("=== SHOW_ALLOC_MEM TEST ===\n\n");
	
	printf("--- Initial state (should show nothing) ---\n");
	show_alloc_mem();
	printf("\n");
	
	// Test 1: TINY allocations
	printf("--- After allocating 3 tiny blocks ---\n");
	char *tiny1 = malloc(10);
	char *tiny2 = malloc(42);
	char *tiny3 = malloc(84);
	strcpy(tiny1, "Hello");
	strcpy(tiny2, "World");
	strcpy(tiny3, "Testing");
	show_alloc_mem();
	printf("\n");
	
	// Test 2: Add SMALL allocations
	printf("--- After adding 2 small blocks ---\n");
	char *small1 = malloc(256);
	char *small2 = malloc(512);
	memset(small1, 'A', 256);
	memset(small2, 'B', 512);
	show_alloc_mem();
	printf("\n");
	
	// Test 3: Add LARGE allocations
	printf("--- After adding 2 large blocks ---\n");
	void *large1 = malloc(5000);
	void *large2 = malloc(10000);
	memset(large1, 'X', 5000);
	memset(large2, 'Y', 10000);
	show_alloc_mem();
	printf("\n");
	
	// Test 4: After freeing some blocks
	printf("--- After freeing tiny2 and small1 ---\n");
	free(tiny2);
	free(small1);
	show_alloc_mem();
	printf("\n");
	
	// Test 5: After freeing all large blocks
	printf("--- After freeing all large blocks ---\n");
	free(large1);
	free(large2);
	show_alloc_mem();
	printf("\n");
	
	// Test 6: After freeing everything
	printf("--- After freeing everything ---\n");
	free(tiny1);
	free(tiny3);
	free(small2);
	show_alloc_mem();
	printf("\n");
	
	// Test 7: Allocate again after freeing everything
	printf("--- Allocate one block after clearing all ---\n");
	void *new_block = malloc(100);
	memset(new_block, 'Z', 100);
	show_alloc_mem();
	printf("\n");
	
	// Cleanup
	free(new_block);
	
	printf("=== TEST COMPLETE ===\n");
	return 0;
}
