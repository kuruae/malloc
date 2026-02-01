#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

void *malloc(size_t size);
void free(void *ptr);

#define NUM_THREADS 8
#define ALLOCS_PER_THREAD 1000
#define TINY_SIZE 64
#define SMALL_SIZE 512

typedef struct {
	int thread_id;
	int success;
} thread_data_t;

void *thread_worker(void *arg) {
	thread_data_t *data = (thread_data_t *)arg;
	void *ptrs[ALLOCS_PER_THREAD];
	
	// Allocate memory
	for (int i = 0; i < ALLOCS_PER_THREAD; i++) {
		size_t size = (i % 2 == 0) ? TINY_SIZE : SMALL_SIZE;
		ptrs[i] = malloc(size);
		if (!ptrs[i]) {
			printf("Thread %d: malloc failed at iteration %d\n", data->thread_id, i);
			data->success = 0;
			return NULL;
		}
		memset(ptrs[i], data->thread_id, size);
	}
	
	// Verify and free
	for (int i = 0; i < ALLOCS_PER_THREAD; i++) {
		size_t size = (i % 2 == 0) ? TINY_SIZE : SMALL_SIZE;
		unsigned char *ptr = (unsigned char *)ptrs[i];
		
		// Verify data integrity
		for (size_t j = 0; j < size; j++) {
			if (ptr[j] != (unsigned char)data->thread_id) {
				printf("Thread %d: data corruption at allocation %d, byte %zu\n", 
				       data->thread_id, i, j);
				data->success = 0;
				return NULL;
			}
		}
		
		free(ptrs[i]);
	}
	
	data->success = 1;
	return NULL;
}

int main(void) {
	pthread_t threads[NUM_THREADS];
	thread_data_t thread_data[NUM_THREADS];
	
	printf("=================================\n");
	printf("   Thread Safety Test Suite\n");
	printf("=================================\n\n");
	printf("Starting %d threads, each doing %d allocations...\n", 
	       NUM_THREADS, ALLOCS_PER_THREAD);
	
	// Create threads
	for (int i = 0; i < NUM_THREADS; i++) {
		thread_data[i].thread_id = i;
		thread_data[i].success = 0;
		
		if (pthread_create(&threads[i], NULL, thread_worker, &thread_data[i]) != 0) {
			printf("✗ FAILED: Could not create thread %d\n", i);
			return 1;
		}
	}
	
	// Wait for threads
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	
	// Check results
	int passed = 0;
	for (int i = 0; i < NUM_THREADS; i++) {
		if (thread_data[i].success) {
			printf("Thread %d: ✓ PASSED\n", i);
			passed++;
		} else {
			printf("Thread %d: ✗ FAILED\n", i);
		}
	}
	
	printf("\n=================================\n");
	printf("Results: %d/%d threads succeeded\n", passed, NUM_THREADS);
	printf("=================================\n");
	
	if (passed == NUM_THREADS) {
		printf("✓ All threads completed successfully!\n");
		printf("✓ No race conditions detected\n");
		printf("✓ Data integrity maintained\n");
		return 0;
	} else {
		printf("✗ Some threads failed\n");
		return 1;
	}
}
