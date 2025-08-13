#include "malloc_extension.h"
#include "malloc.h"
#include "strings.h"

void* calloc(size_t num_elements, size_t element_size) {
    size_t bytes = num_elements * element_size;
    void* ret = malloc(bytes);
    if (ret != NULL) {
        memset(ret, 0, bytes);
    }
    return ret;
}

int atoi(const char *str) { 
    return strtonum(str, NULL); 
}

int abs(int x) {
    return x < 0 ? x * -1 : x;
}