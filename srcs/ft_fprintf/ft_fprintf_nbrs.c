#include "ft_fprintf.h"

int fprint_base_digit_lowercase(int fd, long nb, int base) {
  int count;
  char *baseset;

  count = 0;
  baseset = "0123456789abcdef";
  if (nb < 0) {
    count += fprint_char(fd, '-');
    nb = -nb;
  }
  if (nb >= base)
    count += fprint_base_digit_lowercase(fd, nb / base, base);
  count += fprint_char(fd, baseset[nb % base]);
  return (count);
}

int fprint_base_digit_uppercase(int fd, long nb, int base) {
  int count;
  char *baseset;

  count = 0;
  baseset = "0123456789ABCDEF";
  if (nb < 0) {
    count += fprint_char(fd, '-');
    nb = -nb;
  }
  if (nb >= base)
    count += fprint_base_digit_uppercase(fd, nb / base, base);
  count += fprint_char(fd, baseset[nb % base]);
  return (count);
}

int fprint_base_address(int fd, uintptr_t nb, int base) {
  int count;
  char *baseset;

  count = 0;
  baseset = "0123456789abcdef";
  if (nb >= (uintptr_t)base)
    count += fprint_base_digit_lowercase(fd, nb / base, base);
  count += fprint_char(fd, baseset[nb % base]);
  return (count);
}

int fprint_addr(int fd, void *ptr) {
  int count;
  uintptr_t addr;

  count = 0;
  addr = (uintptr_t)ptr;
  if (!addr)
    count += fprint_string(fd, "(nil)");
  else {
    count += fprint_string(fd, "0x");
    count += fprint_base_address(fd, addr, 16);
  }
  return (count);
}
