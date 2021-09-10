#include "Enclave.h"


// @expect error
// Line 15
// Reason: ret can point to the trusted memory


void serilize1(int *ret)
{
    if (ret == NULL)
    {
        return;
    }
    *ret = 5;
}

void ecall_check_usercheck1_error(uint64_t ret)
{
    int *c = (int *)ret;
    serilize1(c);
}