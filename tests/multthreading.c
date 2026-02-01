#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define THREAD_COUNT 10
#define ITERATIONS 5000
#define ALLOC_SIZE 64

void *stress_test(void *arg) {
    (void)arg;
    void *ptrs[ITERATIONS];

    for (int i = 0; i < ITERATIONS; i++) {
        ptrs[i] = malloc(ALLOC_SIZE);
        if (!ptrs[i]) {
            write(2, "Malloc failed\n", 14);
            return NULL;
        }
        memset(ptrs[i], 0xAA, ALLOC_SIZE);
    }

    for (int i = 0; i < ITERATIONS; i++) {
        free(ptrs[i]);
    }
    
    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];

    write(1, "Starting MT Stress Test...\n", 27);

    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&threads[i], NULL, stress_test, NULL) != 0) {
            write(2, "Thread creation failed\n", 23);
            return 1;
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    write(1, "Test finished. If no crash/error, MT logic is likely safe.\n", 59);
    return 0;
}