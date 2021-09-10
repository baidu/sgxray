#include "Enclave.h"

void ecall_test_validate_pointer_error()
{
    InputBuffer1 *input_buf = (InputBuffer1 *)malloc(sizeof(InputBuffer1));
    if (input_buf == NULL)
    {
        return;
    }
    if (input_buf->buffer_p != NULL)
    {
        *input_buf->buffer_p = 1;
    }
}