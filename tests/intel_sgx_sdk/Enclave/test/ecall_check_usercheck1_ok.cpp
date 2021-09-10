#include "Enclave.h"


// @expect verified


void serilize2(int *ret)
{
    if (ret == NULL)
    {
        return;
    }
    *ret = 5;
}

void ecall_check_usercheck1_ok(uint64_t ret)
{
    int *c = (int *)ret;
    if (!sgx_is_outside_enclave(c, sizeof(int *)))
    {
        return;
    }
    serilize2(c);
}