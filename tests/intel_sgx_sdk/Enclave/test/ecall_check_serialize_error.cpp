#include "Enclave.h"

// @expect error
// Line 14 or line 20
// Reason: the serilization process can overread the buffer.

void ecall_check_serialize_error(char *buffer, size_t size)
{
    if (!buffer)
        return;
    const char *ptr = (const char *)(buffer);
    const char *end_ptr = ptr + size;
    while (ptr < end_ptr)
    {
        uint64_t extent_len;
        memcpy(&extent_len, ptr, sizeof(uint64_t));
        ptr += sizeof(uint64_t);
        char *extent_data = (char *)malloc(extent_len);
        if(extent_data == NULL){
            abort();
        }
        memcpy(extent_data, ptr, extent_len);
        ptr += extent_len;
        free(extent_data);
    }
}
