#ifndef FT_FPRINTF_H
#define FT_FPRINTF_H

#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int ft_fprintf(int fd, const char *format, ...);
int fprint_char(int fd, int c);
int fprint_string(int fd, char *s);
int fprint_base_digit_uppercase(int fd, long nb, int base);
int fprint_base_digit_lowercase(int fd, long nb, int base);
int fprint_base_address(int fd, uintptr_t nb, int base);
int fprint_addr(int fd, void *ptr);
int fparsing_format(int fd, char op, va_list ap);

#endif
