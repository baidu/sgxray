#include "Enclave.h"

void ecall_null_pointer_derefence_ok()
{
    InputBuffer1 *input_buf = (InputBuffer1 *)malloc(sizeof(InputBuffer1));
    if (input_buf == NULL)
    {
        return;
    }
    if (input_buf->buffer_p != NULL)
    {
        printf("Null Pointer");
    }
}
