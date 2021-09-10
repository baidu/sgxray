#include "Enclave.h"


// @expect error
// Line 15 or line 19
// Reason: ret can point to the trusted memory

void ecall_check_usercheck_error(int num, int *ret)
{
    if (ret == NULL)
    {
        return;
    }
    if (num % 2 == 0)
    {
        *ret = 0;
    }
    else
    {
        *ret = 1;
    }
}