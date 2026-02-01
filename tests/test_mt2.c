#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 4
#define ITERS 1000
#define MAX_SIZE 256

static void *thread_func(void *arg) {
  (void)arg;
  void *ptrs[32];
  for (int i = 0; i < ITERS; i++) {
    size_t n = (size_t)(rand() % MAX_SIZE) + 1;
    for (int j = 0; j < 32; j++) {
      ptrs[j] = malloc(n);
      if (ptrs[j])
        memset(ptrs[j], 0xab, n);
    }
    for (int j = 0; j < 32; j++)
      free(ptrs[j]);
  }
  return NULL;
}

int main(void) {
  void *p = malloc(64);
  if (!p) {
    fprintf(stderr, "malloc(64) failed\n");
    return 1;
  }
  memset(p, 0, 64);
  free(p);
  free(NULL);

  /* Large allocation */
  p = malloc(4096);
  if (!p) {
    fprintf(stderr, "malloc(4096) failed\n");
    return 1;
  }
  free(p);

  /* Multi-threaded stress */
  pthread_t threads[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; i++)
    if (pthread_create(&threads[i], NULL, thread_func, NULL) != 0) {
      fprintf(stderr, "pthread_create failed\n");
      return 1;
    }
  for (int i = 0; i < NUM_THREADS; i++)
    pthread_join(threads[i], NULL);

  printf("ok\n");
  return 0;
}
