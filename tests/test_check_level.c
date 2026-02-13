#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *malloc(size_t size);
void free(void *ptr);

int main(void) {
	printf("=================================\n");
	printf("   M_CHECK_LEVEL Test\n");
	printf("=================================\n\n");
	
	void *ptr = malloc(100);
	if (!ptr) {
		printf("✗ FAILED: malloc returned NULL\n");
		return 1;
	}
	
	printf("Allocated 100 bytes at %p\n", ptr);
	printf("Freeing once (should succeed)...\n");
	free(ptr);
	printf("✓ First free succeeded\n\n");
	
	printf("Testing double-free behavior:\n");
	printf("- M_CHECK_LEVEL=0: warns and continues\n");
	printf("- M_CHECK_LEVEL=1: aborts with error\n");
	printf("- M_CHECK_LEVEL=2: aborts with paranoid checks\n\n");
	
	printf("Attempting double-free...\n");
	free(ptr);
	
	printf("✓ Double-free handled (program did not abort)\n");
	printf("This means M_CHECK_LEVEL=0 (default warn mode)\n");
	
	return 0;
}
