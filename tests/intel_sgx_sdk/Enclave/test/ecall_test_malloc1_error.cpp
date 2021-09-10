#include "Enclave.h"

// @expect error
// Line 18
// Reason: The code fails to validat the pointer `result`

void ecall_test_malloc1_error()
{
    uint64_t size = 100;
    if (size > 0xff)
        return;
    void *result;
    uint64_t untrusted_buffers_size = sizeof(uint64_t) * size;
    sgx_status_t status = ocall_untrusted_local_alloc(&result, untrusted_buffers_size);
    if (status != SGX_SUCCESS || result == NULL)
        return;
    uint64_t *untrusted_buffers = (uint64_t *)result;
    for (uint64_t i = 0; i < size; ++i)
    {
        untrusted_buffers[i] = i;
    }
    ocall_untrusted_local_free(untrusted_buffers);
}

