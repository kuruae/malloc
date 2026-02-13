/* Test double-free and error handling */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>

static jmp_buf jump_buffer;
static volatile int signal_received = 0;

void signal_handler(int sig) {
    signal_received = sig;
    longjmp(jump_buffer, 1);
}

int main(void) {
    printf("=== Error Handling Tests ===\n\n");
    
    // Test 1: Double free detection
    printf("Test 1: Double-free detection\n");
    {
        void *ptr = malloc(100);
        printf("  Allocated: %p\n", ptr);
        free(ptr);
        printf("  First free done\n");
        // This should be detected as double-free
        free(ptr);
        printf("  Second free done (should have warned)\n");
    }
    printf("\n");
    
    // Test 2: Wild pointer (never allocated)
    printf("Test 2: Wild pointer detection\n");
    {
        void *fake_ptr = (void*)0x12345678;
        printf("  Attempting to free wild pointer: %p\n", fake_ptr);
        free(fake_ptr);
        printf("  Wild pointer free handled\n");
    }
    printf("\n");
    
    // Test 3: Stack pointer
    printf("Test 3: Stack pointer detection\n");
    {
        char stack_var[100] = "stack data";
        printf("  Attempting to free stack pointer: %p\n", (void*)stack_var);
        free(stack_var);
        printf("  Stack pointer free handled\n");
    }
    printf("\n");
    
    // Test 4: Offset into allocation
    printf("Test 4: Offset pointer detection\n");
    {
        char *ptr = malloc(100);
        printf("  Allocated: %p\n", (void*)ptr);
        printf("  Attempting to free offset pointer: %p\n", (void*)(ptr + 10));
        free(ptr + 10);  // Offset by 10 bytes
        printf("  Offset pointer free handled\n");
        free(ptr);  // Free the actual allocation
    }
    printf("\n");
    
    // Test 5: NULL is always safe
    printf("Test 5: NULL pointer\n");
    {
        free(NULL);
        printf("  NULL free handled correctly\n");
    }
    printf("\n");
    
    // Test 6: Realloc with invalid pointer
    printf("Test 6: Realloc with freed pointer\n");
    {
        void *ptr = malloc(50);
        printf("  Allocated: %p\n", ptr);
        free(ptr);
        printf("  Freed the pointer\n");
        // This is UB but let's see how it handles it
        void *result = realloc(ptr, 100);
        printf("  Realloc on freed pointer returned: %p\n", result);
        if (result) free(result);
    }
    printf("\n");
    
    printf("=== All error handling tests completed ===\n");
    return 0;
}
