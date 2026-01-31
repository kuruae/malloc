#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test edge cases and boundary conditions
int main(void) {
	printf("=== EDGE CASE TESTS ===\n\n");
	
	// Test 1: Zero size allocation
	printf("Test 1: malloc(0)\n");
	void *zero = malloc(0);
	if (zero != NULL) {
		printf("  WARNING: malloc(0) returned non-NULL (behavior varies)\n");
		free(zero);
	} else {
		printf("  SUCCESS: malloc(0) returned NULL\n");
	}
	
	// Test 2: Size 1 allocation
	printf("\nTest 2: Minimal allocation (1 byte)\n");
	char *one = malloc(1);
	if (!one) {
		printf("  FAIL: malloc(1) returned NULL\n");
		return 1;
	}
	*one = 'X';
	if (*one != 'X') {
		printf("  FAIL: Cannot write to 1-byte allocation\n");
		return 1;
	}
	printf("  SUCCESS: 1-byte allocation works\n");
	free(one);
	
	// Test 3: Boundary between tiny and small
	printf("\nTest 3: Boundary allocations (127, 128, 129 bytes)\n");
	void *b127 = malloc(127);
	void *b128 = malloc(128);
	void *b129 = malloc(129);
	
	if (!b127 || !b128 || !b129) {
		printf("  FAIL: Boundary allocations failed\n");
		return 1;
	}
	
	// Write to each to ensure they work
	memset(b127, 'A', 127);
	memset(b128, 'B', 128);
	memset(b129, 'C', 129);
	
	printf("  SUCCESS: All boundary allocations work\n");
	free(b127);
	free(b128);
	free(b129);
	
	// Test 4: Boundary between small and large
	printf("\nTest 4: Large boundary (1000, 1024, 1025 bytes)\n");
	void *l1000 = malloc(1000);
	void *l1024 = malloc(1024);
	void *l1025 = malloc(1025);
	
	if (!l1000 || !l1024 || !l1025) {
		printf("  FAIL: Large boundary allocations failed\n");
		return 1;
	}
	printf("  SUCCESS: Large boundary allocations work\n");
	free(l1000);
	free(l1024);
	free(l1025);
	
	// Test 5: Very large allocation
	printf("\nTest 5: Very large allocation (1MB)\n");
	void *huge = malloc(1024 * 1024);
	if (!huge) {
		printf("  FAIL: 1MB allocation failed\n");
		return 1;
	}
	// Write at boundaries to ensure it's really allocated
	((char*)huge)[0] = 'S';
	((char*)huge)[1024 * 1024 - 1] = 'E';
	
	if (((char*)huge)[0] != 'S' || ((char*)huge)[1024 * 1024 - 1] != 'E') {
		printf("  FAIL: Large allocation memory corrupted\n");
		return 1;
	}
	printf("  SUCCESS: 1MB allocation works\n");
	free(huge);
	
	// Test 6: Many small allocations (zone exhaustion)
	printf("\nTest 6: Many allocations (stress test)\n");
	void *ptrs[100];
	int count = 0;
	for (int i = 0; i < 100; i++) {
		ptrs[i] = malloc(64);
		if (ptrs[i]) {
			count++;
		}
	}
	
	if (count < 100) {
		printf("  WARNING: Only %d/100 allocations succeeded\n", count);
	} else {
		printf("  SUCCESS: All 100 allocations succeeded\n");
	}
	
	// Free all
	for (int i = 0; i < count; i++) {
		free(ptrs[i]);
	}
	
	// Test 7: Alignment check
	printf("\nTest 7: Pointer alignment check\n");
	void *p1 = malloc(1);
	void *p2 = malloc(7);
	void *p3 = malloc(17);
	
	// Check if pointers are aligned to 16 bytes (common alignment)
	if (((unsigned long)p1 % 16) != 0 ||
	    ((unsigned long)p2 % 16) != 0 ||
	    ((unsigned long)p3 % 16) != 0) {
		printf("  WARNING: Pointers not aligned to 16 bytes\n");
	} else {
		printf("  SUCCESS: All pointers aligned to 16 bytes\n");
	}
	
	free(p1);
	free(p2);
	free(p3);
	
	// Test 8: Alternating alloc/free pattern
	printf("\nTest 8: Alternating alloc/free pattern\n");
	for (int i = 0; i < 10; i++) {
		void *temp = malloc(100);
		if (!temp) {
			printf("  FAIL: Alternating pattern allocation %d failed\n", i);
			return 1;
		}
		memset(temp, i, 100);
		free(temp);
	}
	printf("  SUCCESS: Alternating pattern works\n");
	
	printf("\n=== ALL EDGE CASE TESTS PASSED ===\n");
	return 0;
}
