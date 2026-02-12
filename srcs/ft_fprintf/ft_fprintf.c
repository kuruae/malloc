#include "ft_fprintf.h"

int fparsing_format(int fd, char format, va_list ap) {
  int count;

  count = 0;
  if (format == 'c')
    count += fprint_char(fd, va_arg(ap, int));
  else if (format == 's')
    count += fprint_string(fd, va_arg(ap, char *));
  else if (format == 'p')
    count += fprint_addr(fd, va_arg(ap, void *));
  else if (format == 'd' || format == 'i')
    count += fprint_base_digit_lowercase(fd, (long)va_arg(ap, int), 10);
  else if (format == 'u')
    count += fprint_base_digit_lowercase(fd, va_arg(ap, unsigned int), 10);
  else if (format == 'x')
    count +=
        fprint_base_digit_lowercase(fd, (long)va_arg(ap, unsigned int), 16);
  else if (format == 'X')
    count +=
        fprint_base_digit_uppercase(fd, (long)va_arg(ap, unsigned int), 16);
  else if (format == '%')
    count += fprint_char(fd, '%');
  return (count);
}

int ft_fprintf(int fd, const char *format, ...) {
  va_list ap;
  int count;

  va_start(ap, format);
  count = 0;
  if (!format)
    return (-1);
  while (*format) {
    if (*format == '%')
      count += fparsing_format(fd, (*(++format)), ap);
    else
      count += write(fd, format, 1);
    format++;
  }
  va_end(ap);
  return (count);
}
