#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test tiny allocations (typically <= 128 bytes on 4KB page systems)
int main(void) {
	printf("=== TINY ALLOCATION TESTS ===\n\n");
	
	// Test 1: Single tiny allocation
	printf("Test 1: Single tiny allocation (10 bytes)\n");
	char *ptr1 = malloc(10);
	if (!ptr1) {
		printf("  FAIL: malloc(10) returned NULL\n");
		return 1;
	}
	strcpy(ptr1, "Hello");
	printf("  SUCCESS: Allocated and wrote '%s'\n", ptr1);
	
	// Test 2: Multiple tiny allocations
	printf("\nTest 2: Multiple tiny allocations\n");
	char *ptrs[10];
	for (int i = 0; i < 10; i++) {
		ptrs[i] = malloc(16);
		if (!ptrs[i]) {
			printf("  FAIL: malloc(16) #%d returned NULL\n", i);
			return 1;
		}
		sprintf(ptrs[i], "tiny_%d", i);
	}
	printf("  SUCCESS: Allocated 10 tiny blocks\n");
	
	// Verify data integrity
	for (int i = 0; i < 10; i++) {
		char expected[16];
		sprintf(expected, "tiny_%d", i);
		if (strcmp(ptrs[i], expected) != 0) {
			printf("  FAIL: Data corrupted at block %d\n", i);
			return 1;
		}
	}
	printf("  SUCCESS: All data intact\n");
	
	// Test 3: Varying tiny sizes
	printf("\nTest 3: Varying tiny sizes (1, 8, 32, 64, 100 bytes)\n");
	void *tiny1 = malloc(1);
	void *tiny8 = malloc(8);
	void *tiny32 = malloc(32);
	void *tiny64 = malloc(64);
	void *tiny100 = malloc(100);
	
	if (!tiny1 || !tiny8 || !tiny32 || !tiny64 || !tiny100) {
		printf("  FAIL: One or more allocations failed\n");
		return 1;
	}
	printf("  SUCCESS: Allocated various tiny sizes\n");
	
	// Test 4: Free and verify we can still use other pointers
	printf("\nTest 4: Freeing tiny allocations\n");
	free(ptr1);
	free(tiny1);
	free(tiny8);
	printf("  SUCCESS: Freed some pointers, others still valid\n");
	
	// Verify remaining pointers still work
	if (strcmp(ptrs[5], "tiny_5") != 0) {
		printf("  FAIL: Data corrupted after free\n");
		return 1;
	}
	printf("  SUCCESS: Remaining data still intact\n");
	
	// Cleanup
	for (int i = 0; i < 10; i++) {
		free(ptrs[i]);
	}
	free(tiny32);
	free(tiny64);
	free(tiny100);
	
	printf("\n=== ALL TINY TESTS PASSED ===\n");
	return 0;
}
