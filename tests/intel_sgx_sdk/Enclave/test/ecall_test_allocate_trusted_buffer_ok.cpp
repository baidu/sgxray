#include "Enclave.h"

// @expect verified


void ecall_test_allocate_trusted_buffer_ok(){
    int* buffer;
    buffer = (int *)malloc(sizeof(int) * 16);
    if(buffer == NULL){
        return;
    }

    for (int i = 0; i < 16; ++i){
        buffer[i] = i;
    }
}