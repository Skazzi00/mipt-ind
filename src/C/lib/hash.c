#include <stdlib.h>

unsigned hash(void *ptr, size_t size) {
    unsigned char * data = ptr;
    unsigned hash = 5381;
    for (size_t i = 0; i < size; ++i) {
        hash = ((hash << 5u) + hash) + data[i];
    }
    return hash;
}
