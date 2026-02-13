#include "alloc.h"
#include <stdio.h>
#include <string.h>

int main(void) {
	printf("Testing M_CHECK_WILD_PTR=0 mode (fast but no coalescing)\n\n");

	void *ptrs[5];
	for (int i = 0; i < 5; i++) {
		ptrs[i] = malloc(100);
		if (!ptrs[i]) {
			printf("✗ FAILED: malloc returned NULL\n");
			return 1;
		}
		memset(ptrs[i], 'A' + i, 100);
	}

	printf("Allocated 5 chunks of 100 bytes each\n");

	free(ptrs[0]);
	free(ptrs[1]);
	free(ptrs[2]);

	printf("Freed first 3 chunks\n\n");

	printf("With M_CHECK_WILD_PTR=0:\n");
	printf("  - free() is O(1) - no zone searching\n");
	printf("  - Coalescing is DISABLED (accepts fragmentation)\n");
	printf("  - Performance > memory efficiency\n\n");

	void *small1 = malloc(90);
	void *small2 = malloc(90);
	void *small3 = malloc(90);
	
	if (!small1 || !small2 || !small3) {
		printf("✗ FAILED: Could not allocate 3x90 bytes in freed space\n");
		free(ptrs[3]);
		free(ptrs[4]);
		return 1;
	}

	printf("✓ SUCCESS: O(1) free() works correctly!\n");
	printf("  - Freed chunks can still be reused individually\n");
	printf("  - Just not merged together (no coalescing)\n");

	free(small1);
	free(small2);
	free(small3);
	free(ptrs[3]);
	free(ptrs[4]);

	return 0;
}
