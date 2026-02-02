#include "alloc.h"

void __attribute__((constructor)) constructor();

void constructor() {
   ft_putendl_fd("this is a test", 1);
}
