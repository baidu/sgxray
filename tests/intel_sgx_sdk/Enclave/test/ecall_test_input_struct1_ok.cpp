#include "Enclave.h"

void ecall_test_input_struct1_ok(InputBuffer1 *input_buf)
{
    if (!input_buf)
        return;
    size_t buf_size = input_buf->size;
    char *buf = input_buf->buffer_p;
    char *buf_t = (char *)malloc(buf_size);

    if (buf_t == NULL)
    {
        abort();
    }
    if (!buf || !sgx_is_outside_enclave(buf, buf_size))
    {
        return;
    }

    memcpy(buf_t, buf, buf_size);
    free(buf_t);
}
