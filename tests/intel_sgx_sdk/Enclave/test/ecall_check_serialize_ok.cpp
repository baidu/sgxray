#include "Enclave.h"

// @expect verified

void ecall_check_serialize_ok(char *buffer, size_t size)
{
    if (!buffer)
        return;
    const char *ptr = (const char *)(buffer);
    const char *end_ptr = ptr + size;

    while (ptr + sizeof(uint64_t) < end_ptr)
    {
        uint64_t extent_len;
        memcpy(&extent_len, ptr, sizeof(uint64_t));
        ptr += sizeof(uint64_t);
        if (ptr + extent_len > end_ptr)
        {
            // |extent_len| overflows size. This indicates an invalid/modified
            // buffer.
            abort();
        }
        char *extent_data = (char *)malloc(extent_len);
        if (extent_data == NULL)
        {
            abort();
        }
        memcpy(extent_data, ptr, extent_len);
        ptr += extent_len;
        free(extent_data);
    }
}
