#include "Enclave.h"

// @expect verified

void ecall_test_echo_ok()
{
    uint32_t buf[1024];
    uint32_t receive_len;
    sgx_status_t status;
    status = ocall_recv(&receive_len, buf, 1024u);
    if (receive_len > 0 && (status == SGX_SUCCESS))
    {
        receive_len = (receive_len > 1024) ? 1024 : receive_len;
        uint32_t send_len;
        status = ocall_send(&send_len, buf, receive_len);
        if (send_len > 0 && (status == SGX_SUCCESS))
        {
            return;
        }
    }
    abort();
}