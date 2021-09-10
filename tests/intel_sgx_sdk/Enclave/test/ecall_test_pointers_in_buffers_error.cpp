#include "Enclave.h"


void ecall_test_pointers_in_buffers_error(uint64_t * buf, uint64_t num){
    if(!sgx_is_outside_enclave(buf, num*sizeof(uint64_t *))){
        return;
    }

    for(uint64_t i = 0; i < num; ++i){
        uint64_t *ptr;
        memcpy(&ptr, buf, sizeof(uint64_t *));
        *ptr = 0;
        ++buf;
    }
}
