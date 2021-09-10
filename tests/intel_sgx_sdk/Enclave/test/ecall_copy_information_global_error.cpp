#include "Enclave.h"

// @expect error
// Line 18
// Reason: cnt can be controlled by the untrusted world. 
// The code failes to validate the value of cnt

int message1[8] = {0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf};
int secret1[8] = {1, 2, 3, 4, 5, 6, 7, 8};


void ecall_copy_information_global_error(int *ptr, unsigned cnt)
{

    if (!ptr)
        return;

    memcpy(ptr, message1, sizeof(int) * cnt);
}