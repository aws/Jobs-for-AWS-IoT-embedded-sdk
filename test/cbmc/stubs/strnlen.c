#include <stdlib.h>

size_t strnlen(const char * s, size_t maxlen)
{
    __CPROVER_assert(s != NULL, "String pointer must not be null.");

    size_t result;

    __CPROVER_assume(result >= 0 && result <= maxlen);

    return result;
}