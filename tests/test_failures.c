#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test cases that should fail gracefully or expose bugs
int main(void) {
	printf("=== FAILURE & ROBUSTNESS TESTS ===\n\n");
	
	// Test 1: Double free detection (undefined behavior, but shouldn't crash)
	printf("Test 1: Double free (undefined behavior)\n");
	void *ptr = malloc(100);
	if (!ptr) {
		printf("  FAIL: Initial allocation failed\n");
		return 1;
	}
	free(ptr);
	printf("  First free succeeded\n");
	
	// Second free - this is WRONG and may crash or corrupt
	// In a production allocator, this should be detected
	printf("  Attempting double free (may crash or corrupt)...\n");
	free(ptr);
	printf("  WARNING: Double free did not crash (may have corrupted state)\n");
	
	// Test 2: Free invalid pointer (not from malloc)
	printf("\nTest 2: Free invalid pointer\n");
	int stack_var = 42;
	printf("  Attempting to free stack pointer (undefined behavior)...\n");
	free(&stack_var);
	printf("  WARNING: Freeing stack pointer did not crash\n");
	
	// Test 3: Write past allocation boundary
	printf("\nTest 3: Buffer overflow detection\n");
	char *small = malloc(10);
	if (!small) {
		printf("  FAIL: Allocation failed\n");
		return 1;
	}
	
	// Write exactly the allocated amount (OK)
	memset(small, 'X', 10);
	printf("  Writing 10 bytes to 10-byte allocation: OK\n");
	
	// Writing past boundary (buffer overflow - may corrupt next chunk header)
	printf("  WARNING: Writing 11 bytes to 10-byte allocation (overflow)\n");
	memset(small, 'X', 11);  // Intentional overflow
	printf("  Overflow occurred (may corrupt allocator metadata)\n");
	
	// Try to allocate after overflow
	void *after = malloc(20);
	if (after) {
		printf("  WARNING: Allocation after overflow succeeded (metadata might be corrupt)\n");
		free(after);
	} else {
		printf("  FAIL: Allocation after overflow failed (corruption detected)\n");
	}
	free(small);
	
	// Test 4: Use after free
	printf("\nTest 4: Use after free (undefined behavior)\n");
	char *uaf = malloc(50);
	strcpy(uaf, "This will be freed");
	free(uaf);
	
	printf("  Attempting to read freed memory...\n");
	printf("  WARNING: Read freed memory: '%s'\n", uaf);  // May crash or show garbage
	
	printf("  Attempting to write to freed memory...\n");
	strcpy(uaf, "Writing to freed");  // May crash or corrupt
	printf("  WARNING: Write to freed memory succeeded\n");
	
	// Test 5: Extremely large allocation (should fail gracefully)
	printf("\nTest 5: Extremely large allocation (should fail)\n");
	size_t huge_size = (size_t)-1 / 2;  // Very large but not overflow
	void *huge = malloc(huge_size);
	if (huge == NULL) {
		printf("  SUCCESS: Huge allocation correctly returned NULL\n");
	} else {
		printf("  UNEXPECTED: Huge allocation succeeded (%p)\n", huge);
		free(huge);
	}
	
	// Test 6: Allocation size overflow
	printf("\nTest 6: Size overflow (SIZE_MAX)\n");
	void *overflow = malloc((size_t)-1);
	if (overflow == NULL) {
		printf("  SUCCESS: SIZE_MAX allocation correctly returned NULL\n");
	} else {
		printf("  FAIL: SIZE_MAX allocation should have failed\n");
		free(overflow);
		return 1;
	}
	
	// Test 7: Many allocations without free (memory leak test)
	printf("\nTest 7: Memory leak test (allocate without freeing)\n");
	for (int i = 0; i < 1000; i++) {
		void *leak = malloc(100);
		if (!leak) {
			printf("  WARNING: Ran out of memory after %d allocations\n", i);
			break;
		}
		// Intentionally not freeing to test leak detection
	}
	printf("  WARNING: 1000 allocations without free (leaked memory)\n");
	printf("  (Memory will be reclaimed when process exits)\n");
	
	// Test 8: Fragmentation stress test
	printf("\nTest 8: Fragmentation test\n");
	void *frag[50];
	
	// Allocate many blocks
	for (int i = 0; i < 50; i++) {
		frag[i] = malloc(64);
	}
	
	// Free every other block (create fragmentation)
	for (int i = 0; i < 50; i += 2) {
		free(frag[i]);
		frag[i] = NULL;
	}
	
	// Try to allocate larger block (may fail if too fragmented)
	void *large = malloc(1000);
	if (large) {
		printf("  SUCCESS: Large allocation succeeded despite fragmentation\n");
		free(large);
	} else {
		printf("  WARNING: Fragmentation prevented large allocation\n");
	}
	
	// Cleanup remaining blocks
	for (int i = 1; i < 50; i += 2) {
		free(frag[i]);
	}
	
	printf("\n=== FAILURE TESTS COMPLETE ===\n");
	printf("Note: Some tests intentionally trigger undefined behavior\n");
	printf("      A robust allocator should handle these gracefully\n");
	return 0;
}
