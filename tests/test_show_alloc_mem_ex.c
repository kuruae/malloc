#include "alloc.h"
#include <string.h>

int main(void) {
	char *str1 = malloc(50);
	char *str2 = malloc(100);
	int *numbers = malloc(sizeof(int) * 10);
	char *tiny1 = malloc(8);
	char *tiny2 = malloc(16);
	void *large = malloc(1024 * 1024);

	strcpy(str1, "Hello, World!");
	strcpy(str2, "This is a test of the extended memory visualization system.");
	
	for (int i = 0; i < 10; i++)
		numbers[i] = i * 42;
	
	strcpy(tiny1, "TINY");
	strcpy(tiny2, "TINY_ALLOCATION");
	
	memset(large, 0x42, 64);

	show_alloc_mem_ex();

	free(str1);
	free(str2);
	free(numbers);
	free(tiny1);
	free(tiny2);
	free(large);

	return 0;
}
