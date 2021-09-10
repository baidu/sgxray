#include "Enclave.h"

// @expect error
// Line 10
// Reason: cnt can be controlled by the untrusted world. 
// The code failes to validate the value of cnt

void ecall_copy_information_error(int *ptr, unsigned cnt)
{
    int meta[8] = {0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf};
    int __attribute__((unused)) secret[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    memcpy(ptr, meta, sizeof(int) * cnt);
}