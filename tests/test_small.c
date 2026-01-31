#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	printf("=== SMALL ALLOCATION TESTS ===\n\n");
	
	printf("Test 1: Single small allocation (256 bytes)\n");
	char *ptr1 = malloc(256);
	if (!ptr1) {
		printf("  FAIL: malloc(256) returned NULL\n");
		return 1;
	}
	memset(ptr1, 'A', 255);
	ptr1[255] = '\0';
	printf("  SUCCESS: Allocated 256 bytes\n");
	
	printf("\nTest 2: Multiple small allocations (512 bytes each)\n");
	char *ptrs[5];
	for (int i = 0; i < 5; i++) {
		ptrs[i] = malloc(512);
		if (!ptrs[i]) {
			printf("  FAIL: malloc(512) #%d returned NULL\n", i);
			return 1;
		}
		for (int j = 0; j < 512; j++) {
			ptrs[i][j] = (char)((i + j) % 256);
		}
	}
	printf("  SUCCESS: Allocated 5 small blocks\n");
	
	printf("\nTest 3: Verify data integrity\n");
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 512; j++) {
			if (ptrs[i][j] != (char)((i + j) % 256)) {
				printf("  FAIL: Data corrupted at block %d, byte %d\n", i, j);
				return 1;
			}
		}
	}
	printf("  SUCCESS: All data intact across 2560 bytes\n");
	
	printf("\nTest 4: Varying small sizes (200, 400, 600, 800, 1000 bytes)\n");
	void *small200 = malloc(200);
	void *small400 = malloc(400);
	void *small600 = malloc(600);
	void *small800 = malloc(800);
	void *small1000 = malloc(1000);
	
	if (!small200 || !small400 || !small600 || !small800 || !small1000) {
		printf("  FAIL: One or more allocations failed\n");
		return 1;
	}
	printf("  SUCCESS: Allocated various small sizes\n");
	
	printf("\nTest 5: Boundary test (128 and 129 bytes)\n");
	void *boundary1 = malloc(128);  // Likely tiny
	void *boundary2 = malloc(129);  // Likely small
	if (!boundary1 || !boundary2) {
		printf("  FAIL: Boundary allocations failed\n");
		return 1;
	}
	printf("  SUCCESS: Boundary allocations work\n");
	
	printf("\nTest 6: Free and reallocate small blocks\n");
	free(ptrs[2]);
	free(small400);
	
	void *new1 = malloc(512);
	void *new2 = malloc(400);
	if (!new1 || !new2) {
		printf("  FAIL: Reallocation failed\n");
		return 1;
	}
	printf("  SUCCESS: Freed and reallocated successfully\n");
	
	free(ptr1);
	for (int i = 0; i < 5; i++) {
		if (i != 2) free(ptrs[i]);
	}
	free(small200);
	free(small600);
	free(small800);
	free(small1000);
	free(boundary1);
	free(boundary2);
	free(new1);
	free(new2);
	
	printf("\n=== ALL SMALL TESTS PASSED ===\n");
	return 0;
}
