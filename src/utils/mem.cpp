#include "mem.h"
#include <memory.h>

void ZeroMem (void *buffer, size_t size) {
    memset (buffer, 0, size);
}