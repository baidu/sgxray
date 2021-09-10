#include "Enclave.h"

void ecall_test_input_union1_error(InputBuffer2 *input_buf, size_t size)
{
    if (!input_buf)
        return;
    size_t buf_size = size;
    char *buf = input_buf->buffer_p;
    char *buf_t = (char *)malloc(buf_size);

    if (buf_t == NULL)
    {
        abort();
    }

    memcpy(buf_t, buf, buf_size);
    free(buf_t);
}
