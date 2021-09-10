#include "Enclave.h"

int benign1();

int benign2();

int benign3();

int malicious1();

void ecall_check_function_id_error(int *id)
{
    int (*p[4])();
    if (!id || !sgx_is_outside_enclave(id, sizeof(int)))
    {
        return;
    }
    p[0] = benign1;
    p[1] = benign2;
    p[2] = benign3;
    p[3] = malicious1;
    if (*id > 2)
    {
        abort();
    }
    int (*fun_ptr)();
    fun_ptr = p[*id];
    (*fun_ptr)();
}
