#include "Enclave.h"

// @expect error
// Line 15
// 'receive_len' can be controlled by the untrusted world
// The code fails to validate the value of 'receive_len'

void ecall_test_echo_error()
{
    uint32_t buf[1024];
    uint32_t receive_len;
    sgx_status_t status;
    status = ocall_recv(&receive_len, buf, 1024u);
    if (receive_len > 0 && (status == SGX_SUCCESS))
    {
        uint32_t send_len;
        status = ocall_send(&send_len, buf, receive_len);
        if (send_len > 0 && (status == SGX_SUCCESS))
        {
            return;
        }
    }
    abort();
}