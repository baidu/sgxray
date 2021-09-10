#include "Enclave.h"

// @expect verified

void ecall_check_usercheck_ok(int num, int *ret)
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