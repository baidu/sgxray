#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include <pwd.h>
#define MAX_PATH FILENAME_MAX

#include "sgx_urts.h"
#include "App.h"
#include "Enclave_u.h"

void **ocall_enc_untrusted_allocate_buffers(uint64_t count, uint64_t size)
{
    void **buffers = reinterpret_cast<void **>(
        malloc(static_cast<size_t>(count) * sizeof(void *)));
    for (u_int64_t i = 0; i < count; i++)
    {
        buffers[i] = malloc(size);
    }
    return buffers;
}

void ocall_untrusted_local_free(void *buffer)
{
    uint64_t *ptr = static_cast<uint64_t *>(buffer);
    uint64_t value = *ptr;
    printf("free a pointer value %ld\n", value);
    free(buffer);
}



// recv ocall handler
uint32_t ocall_recv(uint32_t *buf, uint32_t size)
{
    uint32_t len_max = 1024;
    if (size < len_max)
    {
        printf("The buffer is two small");
        len_max = size;
    }
    for (uint32_t i = 0; i < len_max; ++i)
    {
        buf[i] = i;
    }
    uint32_t leaked_size = 32;
    return len_max + leaked_size;
}

// send ocall handler
uint32_t ocall_send(uint32_t *buf, uint32_t size)
{
    for (uint32_t i = 0; i < size; ++i)
    {
        //printf("%d ", buf[i]);
    }
    return size;
}

void *ocall_untrusted_local_alloc(uint64_t size)
{
    void *ret = malloc(size);
    if (ret == NULL)
    {
        abort();
    }
    return ret;
}

void ocall_modify_pointer(uint64_t **buf){
    return;
}