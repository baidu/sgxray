#include "Enclave.h"

// @expect error
// Line 40
// TUCTOU

void **allocate_untrusted_buffers(size_t count, size_t size);
void free_buffers(void *buffer_pool_[], int kPoolNumber);

void init_buffers(size_t kPoolNumber, size_t kPoolEntrySize, void * buffer_pool_[])
{
    void **buffers = nullptr;

    buffers = allocate_untrusted_buffers(kPoolNumber, kPoolEntrySize);
    if(buffers == nullptr){
        abort();
    }
    for (size_t i = 0; i < kPoolNumber; i++)
    {
        if (!buffers[i] || !sgx_is_outside_enclave(buffers[i], kPoolEntrySize))
        {
            //printf("OCALL return error %s %d\n", __FILE__, __LINE__);
            abort();
        }
        buffer_pool_[i] = buffers[i];
    }

    ocall_untrusted_local_free(buffers);
}

void ecall_test_allocate_buffers_error()
{
    void * buffer_pool_[16];
    const size_t kPoolEntrySize = sizeof(uint64_t);
    const size_t kPoolNumber = 16;
    //printf("Allocating Buffers\n");
    init_buffers(kPoolNumber, kPoolEntrySize, buffer_pool_);
    for (size_t i = 0; i < kPoolNumber; i++)
    {
        uint64_t *untrusted_ptr = static_cast<uint64_t *>(buffer_pool_[i]);
        *untrusted_ptr = i;
    }

    free_buffers(buffer_pool_, kPoolNumber);
    return;
}
