#include "Enclave.h"

// @expect verified

void ecall_copy_information_heap_ok(int *ptr, unsigned cnt)
{

    if (!ptr)
        return;

    unsigned *message1 = (unsigned *)malloc(cnt * sizeof(unsigned));

    if (message1 == NULL)
        return;

    for (unsigned i = 0; i < cnt; ++i)
    {
        message1[i] = i;
    }

    unsigned *secret1 = (unsigned *)malloc(cnt * sizeof(unsigned));

    if (secret1 == NULL)
        return;

    for (unsigned i = 0; i < cnt; ++i)
    {
        secret1[i] = 0xff;
    }

    memcpy(ptr, message1, sizeof(unsigned) * cnt);

    free(secret1);
    free(message1);
}