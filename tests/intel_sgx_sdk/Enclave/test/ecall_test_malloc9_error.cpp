#include "Enclave.h"

// @expect error
// Line 35
// Reason: The code writes an invalid memory area

void ecall_test_malloc9_error()
{
    uint64_t size = 8;

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
    for (uint64_t i = 0; i < size; ++i)
    {
        untrusted_buffers[i] = i;
    }
    ocall_untrusted_local_free(untrusted_buffers);

    status = ocall_untrusted_local_alloc(&result, untrusted_buffers_size);
    if (status != SGX_SUCCESS || result == NULL)
        return;
    untrusted_buffers = (uint64_t *)result;

    for (uint64_t i = 0; i < size; ++i)
    {
        untrusted_buffers[i] = i;
    }
    ocall_untrusted_local_free(untrusted_buffers);
}
