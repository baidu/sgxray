#include "Enclave.h"

// @expect verified

void ecall_copy_information_in_ok(int *ptr, unsigned cnt)
{
    int meta[8] = {0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf};
    int __attribute__((unused)) secret[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    cnt = (cnt > 8) ? 8 : cnt;
    if (ptr == NULL)
        return;
    memcpy(meta, ptr, sizeof(int) * cnt);
}