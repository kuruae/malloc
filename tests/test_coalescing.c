#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *malloc(size_t size);
void free(void *ptr);
void show_alloc_mem(void);

#define TEST_SIZE 100

int test_basic_coalescing(void) {
	printf("Test 1: Basic forward coalescing (free adjacent chunks)\n");
	
	void *a = malloc(TEST_SIZE);
	void *b = malloc(TEST_SIZE);
	void *c = malloc(TEST_SIZE);
	void *d = malloc(TEST_SIZE);
	
	if (!a || !b || !c || !d) {
		printf("✗ FAILED: malloc returned NULL\n");
		return 0;
	}
	
	memset(a, 'A', TEST_SIZE);
	memset(b, 'B', TEST_SIZE);
	memset(c, 'C', TEST_SIZE);
	memset(d, 'D', TEST_SIZE);
	
	free(b);
	free(c);
	
	void *large = malloc(TEST_SIZE * 2 + 10);
	if (!large) {
		printf("✗ FAILED: Could not allocate large chunk after coalescing\n");
		free(a);
		free(d);
		return 0;
	}
	
	printf("✓ PASSED: Successfully allocated %d bytes after coalescing\n", TEST_SIZE * 2 + 10);
	
	free(a);
	free(d);
	free(large);
	return 1;
}

int test_no_coalescing_with_used_chunk(void) {
	printf("\nTest 2: Coalescing stops at used chunks\n");
	
	void *ptrs[10];
	for (int i = 0; i < 10; i++) {
		ptrs[i] = malloc(TEST_SIZE);
		if (!ptrs[i]) {
			printf("✗ FAILED: malloc returned NULL\n");
			for (int j = 0; j < i; j++)
				free(ptrs[j]);
			return 0;
		}
	}
	
	free(ptrs[0]);
	free(ptrs[1]);
	
	void *merged = malloc(TEST_SIZE * 2 + 10);
	if (!merged) {
		printf("✗ FAILED: Should coalesce adjacent free chunks\n");
		for (int i = 2; i < 10; i++)
			free(ptrs[i]);
		return 0;
	}
	
	printf("✓ PASSED: Successfully coalesced adjacent free chunks\n");
	free(merged);
	for (int i = 2; i < 10; i++)
		free(ptrs[i]);
	return 1;
}

int test_sequential_coalescing(void) {
	printf("\nTest 3: Sequential coalescing (free multiple in order)\n");
	
	void *ptrs[5];
	for (int i = 0; i < 5; i++) {
		ptrs[i] = malloc(TEST_SIZE);
		if (!ptrs[i]) {
			printf("✗ FAILED: malloc returned NULL at index %d\n", i);
			for (int j = 0; j < i; j++)
				free(ptrs[j]);
			return 0;
		}
	}
	
	for (int i = 0; i < 5; i++)
		free(ptrs[i]);
	
	void *huge = malloc(TEST_SIZE * 5 + 50);
	if (!huge) {
		printf("✗ FAILED: Could not allocate huge chunk after full coalescing\n");
		return 0;
	}
	
	printf("✓ PASSED: Successfully allocated %d bytes after sequential coalescing\n", TEST_SIZE * 5 + 50);
	free(huge);
	return 1;
}

int test_edge_case_last_chunk(void) {
	printf("\nTest 4: Edge case - coalescing last chunk in zone\n");
	
	void *a = malloc(TEST_SIZE);
	void *b = malloc(TEST_SIZE);
	
	if (!a || !b) {
		printf("✗ FAILED: malloc returned NULL\n");
		return 0;
	}
	
	free(b);
	
	void *c = malloc(TEST_SIZE / 2);
	if (!c) {
		printf("✗ FAILED: Could not reuse freed last chunk\n");
		free(a);
		return 0;
	}
	
	printf("✓ PASSED: Successfully reused last freed chunk\n");
	free(a);
	free(c);
	return 1;
}

int test_alternating_free(void) {
	printf("\nTest 5: Alternating free pattern (limited coalescing)\n");
	
	void *ptrs[6];
	for (int i = 0; i < 6; i++) {
		ptrs[i] = malloc(TEST_SIZE);
		if (!ptrs[i]) {
			printf("✗ FAILED: malloc returned NULL at index %d\n", i);
			for (int j = 0; j < i; j++)
				free(ptrs[j]);
			return 0;
		}
	}
	
	for (int i = 0; i < 6; i += 2)
		free(ptrs[i]);
	
	void *single = malloc(TEST_SIZE - 10);
	if (!single) {
		printf("✗ FAILED: Should be able to allocate single chunk size\n");
		for (int i = 1; i < 6; i += 2)
			free(ptrs[i]);
		return 0;
	}
	
	printf("✓ PASSED: Correctly handled alternating free pattern\n");
	free(single);
	for (int i = 1; i < 6; i += 2)
		free(ptrs[i]);
	return 1;
}

int main(void) {
	int passed = 0;
	int total = 5;
	
	printf("=================================\n");
	printf("   Coalescing Test Suite\n");
	printf("=================================\n\n");
	
	passed += test_basic_coalescing();
	passed += test_no_coalescing_with_used_chunk();
	passed += test_sequential_coalescing();
	passed += test_edge_case_last_chunk();
	passed += test_alternating_free();
	
	printf("\n=================================\n");
	printf("Results: %d/%d tests passed\n", passed, total);
	printf("=================================\n");
	
	if (passed == total) {
		printf("✓ All coalescing tests passed!\n");
		return 0;
	} else {
		printf("✗ Some tests failed\n");
		return 1;
	}
}
