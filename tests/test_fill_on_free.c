#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Test M_FILL_ON_FREE environment variable
 *
 * Run with: M_FILL_ON_FREE=1 make test TESTFILE=tests/test_fill_on_free.c
 * Run without: M_FILL_ON_FREE=0 make test TESTFILE=tests/test_fill_on_free.c
 */

void print_memory(const char *label, void *ptr, size_t size) {
  unsigned char *p = (unsigned char *)ptr;
  printf("%s: ", label);
  size_t display_size = size < 32 ? size : 32; // Show first 32 bytes max
  for (size_t i = 0; i < display_size; i++) {
    printf("%02x ", p[i]);
    if ((i + 1) % 16 == 0 && i != display_size - 1)
      printf("\n%*s", (int)strlen(label) + 2, "");
  }
  if (size > 32)
    printf("... (%zu more bytes)", size - 32);
  printf("\n");
}

void test_allocation(const char *name, size_t size) {
  printf("\n=== %s (%zu bytes) ===\n", name, size);

  void *ptr = malloc(size);
  if (!ptr) {
    printf("FAILED: malloc returned NULL\n");
    return;
  }

  memset(ptr, 0xAA, size);
  print_memory("Before free", ptr, size);

  free(ptr);

  print_memory("After free ", ptr, size);
}

int main(void) {
  printf("=== M_FILL_ON_FREE TEST ===\n");

  char *env_val = getenv("M_FILL_ON_FREE");
  if (env_val && *env_val == '1') {
    printf("M_FILL_ON_FREE is ENABLED - Expect zeros after free\n");
  } else {
    printf("M_FILL_ON_FREE is DISABLED - Expect pattern 0xAA after free\n");
  }

  test_allocation("TINY", 64);
  test_allocation("SMALL", 512);
  test_allocation("LARGE", 8192);

  printf("\n=== TEST COMPLETED ===\n");
  return 0;
}
