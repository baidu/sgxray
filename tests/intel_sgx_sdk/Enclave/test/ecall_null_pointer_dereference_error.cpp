#include "Enclave.h"


void ecall_null_pointer_derefence_error(){
    InputBuffer1 *input_buf = (InputBuffer1 *)malloc(sizeof(InputBuffer1));
    if (input_buf->buffer_p != NULL){
        printf("Null Pointer");
        char *a = input_buf->buffer_p;
        printf("%p\n", a);
    }
}
