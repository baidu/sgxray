#include "Enclave.h"

// @expect error
// Line 30
// Reason: The code reads an invalid memory area

int pow(int n){
    int result = 1;
    for(int i = 0; i < n; ++i){
        result = 2*result;
    }
    return result;
}

void ecall_test_malloc7_error()
{
    uint64_t size = 100;
    void *result;
    uint64_t untrusted_buffers_size = sizeof(uint64_t) * size;
    sgx_status_t status = ocall_untrusted_local_alloc(&result, untrusted_buffers_size);
    if (status != SGX_SUCCESS || result == NULL)
        return;
    if (!sgx_is_outside_enclave(result, untrusted_buffers_size))
    {
        return;
    }
    uint64_t *untrusted_buffers = (uint64_t *)result;
    for (int i = 0; i < 15; ++i)
    {
        untrusted_buffers[pow(i)] = (uint64_t)i;
    }
    ocall_untrusted_local_free(untrusted_buffers);
}