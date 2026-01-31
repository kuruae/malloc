#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	printf("=== REALLOC() TESTS ===\n\n");
	
	// Test 1: realloc(NULL, size) should act like malloc
	printf("Test 1: realloc(NULL, size) - should act like malloc\n");
	char *ptr1 = realloc(NULL, 50);
	if (!ptr1) {
		printf("  FAIL: realloc(NULL, 50) returned NULL\n");
		return 1;
	}
	strcpy(ptr1, "Created with realloc");
	printf("  SUCCESS: realloc(NULL, 50) = %p\n", (void *)ptr1);
	printf("  Data: '%s'\n", ptr1);
	
	// Test 2: realloc(ptr, 0) should act like free
	printf("\nTest 2: realloc(ptr, 0) - should act like free\n");
	void *result = realloc(ptr1, 0);
	if (result != NULL) {
		printf("  WARNING: realloc(ptr, 0) returned non-NULL\n");
	} else {
		printf("  SUCCESS: realloc(ptr, 0) returned NULL\n");
	}
	
	// Test 3: Grow allocation (preserve data)
	printf("\nTest 3: Grow allocation (100 -> 200 bytes)\n");
	char *ptr2 = malloc(100);
	strcpy(ptr2, "Original data that must be preserved");
	printf("  Original: '%s' at %p\n", ptr2, (void *)ptr2);
	
	char *ptr3 = realloc(ptr2, 200);
	if (!ptr3) {
		printf("  FAIL: realloc failed\n");
		free(ptr2);
		return 1;
	}
	printf("  Reallocated to %p\n", (void *)ptr3);
	
	if (strcmp(ptr3, "Original data that must be preserved") != 0) {
		printf("  FAIL: Data corrupted after realloc\n");
		printf("  Got: '%s'\n", ptr3);
		return 1;
	}
	printf("  SUCCESS: Data preserved: '%s'\n", ptr3);
	
	// Write to new space
	strcat(ptr3, " - and now extended!");
	printf("  Extended data: '%s'\n", ptr3);
	
	// Test 4: Shrink allocation (preserve data that fits)
	printf("\nTest 4: Shrink allocation (200 -> 50 bytes)\n");
	char *ptr4 = realloc(ptr3, 50);
	if (!ptr4) {
		printf("  FAIL: realloc failed\n");
		return 1;
	}
	printf("  Shrunk to 50 bytes at %p\n", (void *)ptr4);
	
	// Data should be truncated but first 50 chars preserved
	ptr4[49] = '\0';  // Ensure null termination
	printf("  First 49 chars: '%s'\n", ptr4);
	
	// Test 5: Realloc to same size
	printf("\nTest 5: Realloc to same size (50 -> 50 bytes)\n");
	char *ptr5 = realloc(ptr4, 50);
	if (!ptr5) {
		printf("  FAIL: realloc failed\n");
		return 1;
	}
	printf("  SUCCESS: realloc(ptr, 50) = %p (same or different OK)\n", (void *)ptr5);
	
	// Test 6: Cross zone boundaries (tiny -> small)
	printf("\nTest 6: Cross zone boundary (tiny 32 bytes -> small 300 bytes)\n");
	char *tiny = malloc(32);
	strcpy(tiny, "Tiny zone data");
	printf("  Original (tiny): '%s' at %p\n", tiny, (void *)tiny);
	
	char *small = realloc(tiny, 300);
	if (!small) {
		printf("  FAIL: realloc failed\n");
		return 1;
	}
	printf("  Reallocated (small): '%s' at %p\n", small, (void *)small);
	
	if (strcmp(small, "Tiny zone data") != 0) {
		printf("  FAIL: Data corrupted crossing zones\n");
		return 1;
	}
	printf("  SUCCESS: Data preserved across zone change\n");
	
	// Test 7: Small to large
	printf("\nTest 7: Cross zone boundary (small 300 bytes -> large 5000 bytes)\n");
	strcpy(small, "Small zone data");
	char *large = realloc(small, 5000);
	if (!large) {
		printf("  FAIL: realloc failed\n");
		return 1;
	}
	printf("  Reallocated (large): '%s' at %p\n", large, (void *)large);
	
	if (strcmp(large, "Small zone data") != 0) {
		printf("  FAIL: Data corrupted crossing to large\n");
		return 1;
	}
	printf("  SUCCESS: Data preserved from small to large\n");
	
	// Test 8: Large to tiny
	printf("\nTest 8: Large back to tiny (5000 -> 16 bytes)\n");
	strcpy(large, "12345678901234");  // 14 chars + null
	char *tiny2 = realloc(large, 16);
	if (!tiny2) {
		printf("  FAIL: realloc failed\n");
		return 1;
	}
	printf("  Reallocated (tiny): '%s' at %p\n", tiny2, (void *)tiny2);
	
	if (strcmp(tiny2, "12345678901234") != 0) {
		printf("  FAIL: Data corrupted\n");
		return 1;
	}
	printf("  SUCCESS: Data preserved from large to tiny\n");
	
	// Test 9: Multiple reallocs in sequence
	printf("\nTest 9: Multiple sequential reallocs\n");
	void *chain = malloc(10);
	printf("  Start: 10 bytes at %p\n", (void *)chain);
	
	chain = realloc(chain, 20);
	printf("  After realloc to 20: %p\n", (void *)chain);
	
	chain = realloc(chain, 100);
	printf("  After realloc to 100: %p\n", (void *)chain);
	
	chain = realloc(chain, 1000);
	printf("  After realloc to 1000: %p\n", (void *)chain);
	
	chain = realloc(chain, 50);
	printf("  After realloc to 50: %p\n", (void *)chain);
	
	printf("  SUCCESS: Chain of reallocs completed\n");
	
	free(ptr5);
	free(tiny2);
	free(chain);
	
	printf("\n=== ALL REALLOC TESTS PASSED ===\n");
	return 0;
}
