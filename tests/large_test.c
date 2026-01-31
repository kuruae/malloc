#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#define SIZE_ARR 6000
typedef int32_t i32;

void print_int(int n) {
    char buf[12];
    int i = 0;

    if (n == 0) {
        write(1, "0", 1);
        return;
    }

    if (n < 0) {
        write(1, "-", 1);
        if (n == INT_MIN) {
            write(1, "2147483648", 10);
            return;
        }
        n = -n;
    }

    while (n > 0) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }

    while (i--) {
        write(1, &buf[i], 1);
    }
}

void print_int_array(i32 *arr, int size) {
    write(1, "[", 1);
    for (int i = 0; i < size; i++) {
        print_int(arr[i]);
        if (i != size - 1)
            write(1, ", ", 2);
    }
    write(1, "]\n", 2);
}

int main(void) {
    i32 *test_arr = malloc(sizeof(*test_arr) * SIZE_ARR);
    if (!test_arr)
        return 1;

    for (int i = 0; i < SIZE_ARR; i++) {
        test_arr[i] = i * 1103515245 + 12345;
    }

    print_int_array(test_arr, SIZE_ARR);
    return 0;
}
