#include "Enclave.h"

// @expect verified

void **allocate_untrusted_buffers(size_t count, size_t size);
void free_buffers(void *buffer_pool_[], int kPoolNumber);

void init_buffers_safe(size_t kPoolNumber, size_t kPoolEntrySize,void * buffer_pool_[])
{
    void **buffers = nullptr;

    buffers = allocate_untrusted_buffers(kPoolNumber, kPoolEntrySize);
    if(buffers == nullptr){
        abort();
    }
    for (size_t i = 0; i < kPoolNumber; i++)
    {
        void *temp = buffers[i];
        if (!temp || !sgx_is_outside_enclave(temp, kPoolEntrySize))
        {
            abort();
        }
        buffer_pool_[i] = temp;
    }

    ocall_untrusted_local_free(buffers);
}

void ecall_test_allocate_buffers_ok()
{
    void * buffer_pool_[16];
    const size_t kPoolEntrySize = sizeof(uint64_t);
    const size_t kPoolNumber = 16;
    init_buffers_safe(kPoolNumber, kPoolEntrySize, buffer_pool_);
    for (size_t i = 0; i < kPoolNumber; i++)
    {
        uint64_t *untrusted_ptr = static_cast<uint64_t *>(buffer_pool_[i]);
        *untrusted_ptr = i;
    }

    free_buffers(buffer_pool_, kPoolNumber);
    return;
}