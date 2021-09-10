#include "Enclave.h"

// @expect verified
int message2[8] = {0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf};
int secret2[8] = {1, 2, 3, 4, 5, 6, 7, 8};


void ecall_copy_information_global_ok(int *ptr, unsigned cnt)
{

    if (!ptr)
        return;

    cnt = (cnt > 8) ? 8 : cnt;
    memcpy(ptr, message2, sizeof(int) * cnt);
}