#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test free() behavior and memory reuse
int main(void) {
	printf("=== FREE() TESTS ===\n\n");
	
	// Test 1: Basic free
	printf("Test 1: Basic free and NULL handling\n");
	void *ptr = malloc(100);
	if (!ptr) {
		printf("  FAIL: malloc failed\n");
		return 1;
	}
	free(ptr);
	printf("  SUCCESS: Basic free works\n");
	
	free(NULL);  // Should not crash
	printf("  SUCCESS: free(NULL) handled correctly\n");
	
	// Test 2: Free in different orders
	printf("\nTest 2: Free in different orders\n");
	void *p1 = malloc(50);
	void *p2 = malloc(50);
	void *p3 = malloc(50);
	void *p4 = malloc(50);
	
	// Free in reverse order
	free(p4);
	free(p3);
	free(p2);
	free(p1);
	printf("  SUCCESS: Freed in reverse order\n");
	
	// Allocate again
	p1 = malloc(50);
	p2 = malloc(50);
	p3 = malloc(50);
	p4 = malloc(50);
	
	// Free in random order
	free(p2);
	free(p4);
	free(p1);
	free(p3);
	printf("  SUCCESS: Freed in random order\n");
	
	// Test 3: Memory reuse after free
	printf("\nTest 3: Memory reuse after free\n");
	void *orig1 = malloc(64);
	void *orig2 = malloc(64);
	void *orig3 = malloc(64);
	
	// Write data
	memset(orig1, 'X', 64);
	memset(orig2, 'Y', 64);
	memset(orig3, 'Z', 64);
	
	// Free middle one
	free(orig2);
	
	// Allocate similar size - might reuse freed space
	void *reused = malloc(64);
	if (!reused) {
		printf("  FAIL: Reallocation failed\n");
		return 1;
	}
	
	// Write new data
	memset(reused, 'R', 64);
	
	// Verify other blocks still intact
	int corrupted = 0;
	for (int i = 0; i < 64; i++) {
		if (((char*)orig1)[i] != 'X' || ((char*)orig3)[i] != 'Z') {
			corrupted = 1;
			break;
		}
	}
	
	if (corrupted) {
		printf("  FAIL: Adjacent blocks corrupted\n");
		return 1;
	}
	printf("  SUCCESS: Memory reused, adjacent blocks intact\n");
	
	// Test 4: Free all from a zone
	printf("\nTest 4: Free many allocations\n");
	void *ptrs[20];
	for (int i = 0; i < 20; i++) {
		ptrs[i] = malloc(32);
		if (!ptrs[i]) {
			printf("  FAIL: Allocation %d failed\n", i);
			return 1;
		}
	}
	
	for (int i = 0; i < 20; i++) {
		free(ptrs[i]);
	}
	printf("  SUCCESS: Freed 20 allocations\n");
	
	// Test 5: Mixed size frees
	printf("\nTest 5: Mixed size frees (tiny, small, large)\n");
	void *tiny = malloc(10);
	void *small = malloc(500);
	void *large = malloc(10000);
	
	if (!tiny || !small || !large) {
		printf("  FAIL: Mixed allocations failed\n");
		return 1;
	}
	
	free(small);
	free(large);
	free(tiny);
	printf("  SUCCESS: Freed different sized allocations\n");
	
	// Test 6: Allocate after freeing everything
	printf("\nTest 6: Allocate after freeing everything\n");
	void *final = malloc(128);
	if (!final) {
		printf("  FAIL: Final allocation failed\n");
		return 1;
	}
	strcpy(final, "Still working!");
	printf("  SUCCESS: Allocated after mass free: '%s'\n", (char*)final);
	
	// Cleanup
	free(orig1);
	free(orig3);
	free(reused);
	free(final);
	
	printf("\n=== ALL FREE TESTS PASSED ===\n");
	return 0;
}
