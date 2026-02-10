#include "ft_fprintf.h"

int fprint_char(int fd, int c) { return (write(fd, &c, 1)); }

int fprint_string(int fd, char *s) {
  int count;

  count = 0;
  if (!s) {
    count = fprint_string(fd, "(null)");
    return (count);
  }
  while (*s) {
    fprint_char(fd, (int)*s);
    s++;
    count++;
  }
  return (count);
}
