#include "Enclave.h"

// @expect error
// Line 31
// Reason: cnt can be controlled by the untrusted world. 
// The code failes to validate the value of cnt


void ecall_copy_information_heap_error(int *ptr, unsigned cnt)
{

    if (!ptr)
        return;

    unsigned *message1 = (unsigned *)malloc((cnt - 1) * sizeof(unsigned));

    if (message1 == NULL) return;

    for(unsigned i = 0; i < cnt - 1; ++i){
        message1[i] = i;
    }

    unsigned *secret1 = (unsigned *)malloc(cnt * sizeof(unsigned));

    if (secret1 == NULL) return;

    for(unsigned i = 0; i < cnt; ++i){
        secret1[i] = 0xff;
    }

    memcpy(ptr, message1, sizeof(unsigned) * cnt);

    free(secret1);
    free(message1);
}