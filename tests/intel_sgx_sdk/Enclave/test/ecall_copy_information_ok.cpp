#include "Enclave.h"

// @expect verified

void ecall_copy_information_ok(int *ptr, unsigned cnt)
{
    int meta[8] = {0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf};
    int __attribute__((unused)) secret[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    printf("meta address %p\n", meta);
    printf("secret address %p\n", secret);
    if (!ptr)
        return;

    cnt = (cnt > 8) ? 8 : cnt;
    memcpy(ptr, meta, sizeof(int) * cnt);
}