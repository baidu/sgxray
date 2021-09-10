#include "Enclave.h"

void **allocate_untrusted_buffers(size_t count, size_t size)
{
    void **buffers;
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    ret = ocall_enc_untrusted_allocate_buffers(
        &buffers, static_cast<uint64_t>(count), size);
    if (ret != SGX_SUCCESS || !sgx_is_outside_enclave(buffers, count * sizeof(void *)))
    {
        printf("OCALL return error %s %d\n", __FILE__, __LINE__);
        abort();
    }
    return buffers;
}

void free_buffers(void *buffer_pool_[], int kPoolNumber)
{
    for (int i = 0; i < kPoolNumber; ++i)
    {
        ocall_untrusted_local_free(buffer_pool_[i]);
    }
}