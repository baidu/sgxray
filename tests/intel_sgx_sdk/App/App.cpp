#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include <pwd.h>
#define MAX_PATH FILENAME_MAX

#include "sgx_urts.h"
#include "App.h"
#include "Enclave_u.h"

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

typedef struct App
{
    int id;
    const char *name;
} sgx_test_case;

static sgx_test_case sgx_test_case_list[]{
    {0, "ecall_copy_information_error"},
    {1, "ecall_copy_information_ok"},
    {2, "ecall_copy_information_in_error"},
    {3, "ecall_copy_information_in_ok"},
    {4, "ecall_check_usercheck_error"},
    {5, "ecall_check_usercheck_ok"},
    {6, "ecall_check_usercheck1_error"},
    {7, "ecall_check_usercheck1_ok"},
    {8, "ecall_test_allocate_buffers_error"},
    {9, "ecall_test_allocate_buffers_ok"},
    {10, "ecall_test_echo_error"},
    {11, "ecall_test_echo_ok"},
    {12, "ecall_test_malloc1_error"},
    {13, "ecall_test_malloc2_error"},
    {14, "ecall_test_malloc3_error"},
    {15, "ecall_test_malloc4_ok"},
    {16, "ecall_test_malloc5_error"},
    {17, "ecall_test_malloc6_ok"},
    {18, "ecall_test_malloc7_error"},
    {19, "ecall_test_malloc8_error"},
    {20, "ecall_check_serialize_ok"},
    {21, "ecall_check_serialize_error"},
    {22, "ecall_check_function_id_error"},
    {23, "ecall_check_function_id_ok"},
    {24, "ecall_test_input_struct1_error"},
    {25, "ecall_test_input_struct1_ok"},
    {26, "ecall_test_input_union1_error"},
    {27, "ecall_test_input_union1_ok"},
    {28, "ecall_copy_information_global_error"},
    {29, "ecall_copy_information_global_ok"},
    {30, "ecall_copy_information_heap_error"},
    {31, "ecall_copy_information_heap_ok"},
    {32, "ecall_test_allocate_trusted_buffer_ok"},
    {33, "ecall_null_pointer_derefence_error"},
    {34, "ecall_null_pointer_derefence_ok"},
    {35, "ecall_test_validate_pointer_error"},
    {36, "ecall_test_pointers_in_buffers_error"}};

typedef struct _sgx_errlist_t
{
    sgx_status_t err;
    const char *msg;
    const char *sug; /* Suggestion */
} sgx_errlist_t;

/* Error code returned by sgx_create_enclave */
static sgx_errlist_t sgx_errlist[] = {
    {SGX_ERROR_UNEXPECTED,
     "Unexpected error occurred.",
     NULL},
    {SGX_ERROR_INVALID_PARAMETER,
     "Invalid parameter.",
     NULL},
    {SGX_ERROR_OUT_OF_MEMORY,
     "Out of memory.",
     NULL},
    {SGX_ERROR_ENCLAVE_LOST,
     "Power transition occurred.",
     "Please refer to the sample \"PowerTransition\" for details."},
    {SGX_ERROR_INVALID_ENCLAVE,
     "Invalid enclave image.",
     NULL},
    {SGX_ERROR_INVALID_ENCLAVE_ID,
     "Invalid enclave identification.",
     NULL},
    {SGX_ERROR_INVALID_SIGNATURE,
     "Invalid enclave signature.",
     NULL},
    {SGX_ERROR_OUT_OF_EPC,
     "Out of EPC memory.",
     NULL},
    {SGX_ERROR_NO_DEVICE,
     "Invalid SGX device.",
     "Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards."},
    {SGX_ERROR_MEMORY_MAP_CONFLICT,
     "Memory map conflicted.",
     NULL},
    {SGX_ERROR_INVALID_METADATA,
     "Invalid enclave metadata.",
     NULL},
    {SGX_ERROR_DEVICE_BUSY,
     "SGX device was busy.",
     NULL},
    {SGX_ERROR_INVALID_VERSION,
     "Enclave version was invalid.",
     NULL},
    {SGX_ERROR_INVALID_ATTRIBUTE,
     "Enclave was not authorized.",
     NULL},
    {SGX_ERROR_ENCLAVE_FILE_ACCESS,
     "Can't open enclave file.",
     NULL},
};

/* Check error conditions for loading enclave */
void print_error_message(sgx_status_t ret)
{
    size_t idx = 0;
    size_t ttl = sizeof sgx_errlist / sizeof sgx_errlist[0];

    for (idx = 0; idx < ttl; idx++)
    {
        if (ret == sgx_errlist[idx].err)
        {
            if (NULL != sgx_errlist[idx].sug)
                printf("Info: %s\n", sgx_errlist[idx].sug);
            printf("Error: %s\n", sgx_errlist[idx].msg);
            break;
        }
    }

    if (idx == ttl)
        printf("Error code is 0x%X. Please refer to the \"Intel SGX SDK Developer Reference\" for more details.\n", ret);
}

/* print test case */
void print_test_cases(int num)
{
    if (num == -1)
    {
        int test_case_size = sizeof(sgx_test_case_list) / sizeof(sgx_test_case_list[0]);
        for (int i = 0; i < test_case_size; ++i)
        {
            printf("id = %d, name = %s\n", sgx_test_case_list[i].id, sgx_test_case_list[i].name);
        }
        return;
    }
    printf("id = %d, name = %s\n", sgx_test_case_list[num].id, sgx_test_case_list[num].name);
    return;
}

/* Initialize the enclave:
 *   Call sgx_create_enclave to initialize an enclave instance
 */
int initialize_enclave(void)
{
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;

    /* Call sgx_create_enclave to initialize an enclave instance */
    /* Debug Support: set 2nd parameter to 1 */
    ret = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, NULL, NULL, &global_eid, NULL);
    if (ret != SGX_SUCCESS)
    {
        print_error_message(ret);
        return -1;
    }

    return 0;
}

/* OCall functions */
void ocall_print_string(const char *str)
{
    /* Proxy/Bridge will check the length and null-terminate 
     * the input string to prevent buffer overflow. 
     */
    printf("%s", str);
}

int test_case_dispatch(int id)
{
    int ret_val = 0;
    print_test_cases(id);
    switch (id)
    {
    case 0:
    {
        int buffer[16];
        unsigned size = 16;
        ecall_copy_information_error(global_eid, buffer, size);
        for (int i = 0; i < 16; ++i)
        {
            printf("Value %d \n", buffer[i]);
        }
        for (int i = 0; i < 16; ++i)
        {
            buffer[i] = 0;
        }
    }
    break;

    case 1:
    {
        int buffer[16];
        unsigned size = 16;
        ecall_copy_information_ok(global_eid, buffer, size);
        for (int i = 0; i < 16; ++i)
        {
            printf("Value %d \n", buffer[i]);
        }
    }
    break;

    case 2:
    {
        int buffer[16] = {0};
        unsigned size = 16;
        ecall_copy_information_in_error(global_eid, buffer, size);
    }
    break;

    case 3:
    {
        int buffer[16] = {0};
        unsigned size = 16;
        ecall_copy_information_in_ok(global_eid, buffer, size);
    }
    break;

    case 4:
    {
        int user_input = 16;
        ecall_check_usercheck_error(global_eid, user_input, &ret_val);
        if (ret_val == 0)
        {
            printf("Input Number is even\n");
        }
        else
        {
            printf("Input Number is odd\n");
        }
    }
    break;

    case 5:
    {
        int user_input = 15;
        ecall_check_usercheck_ok(global_eid, user_input, &ret_val);
        if (ret_val == 0)
        {
            printf("Input Number is even\n");
        }
        else
        {
            printf("Input Number is odd\n");
        }
    }
    break;

    case 6:
    {
        int *data = (int *)malloc(sizeof(int));
        if (data == NULL)
            return -1;
        ecall_check_usercheck1_error(global_eid, (uint64_t)data);
        free(data);
    }
    break;

    case 7:
    {
        int *data = (int *)malloc(sizeof(int));
        if (data == NULL)
            return -1;
        ecall_check_usercheck1_ok(global_eid, (uint64_t)data);
        free(data);
    }
    break;

    case 8:
    {
        ecall_test_allocate_buffers_error(global_eid);
    }
    break;

    case 9:
    {
        ecall_test_allocate_buffers_ok(global_eid);
    }
    break;

    case 10:
    {
        ecall_test_echo_error(global_eid);
    }
    break;

    case 11:
    {
        ecall_test_echo_ok(global_eid);
    }
    break;

    case 12:
    {
        ecall_test_malloc1_error(global_eid);
    }
    break;

    case 13:
    {
        ecall_test_malloc2_error(global_eid);
    }
    break;

    case 14:
    {
        ecall_test_malloc3_error(global_eid);
    }
    break;

    case 15:
    {
        ecall_test_malloc4_ok(global_eid);
    }
    break;

    case 16:
    {
        ecall_test_malloc5_error(global_eid);
    }
    break;

    case 17:
    {
        ecall_test_malloc6_ok(global_eid);
    }
    break;

    case 18:
    {
        ecall_test_malloc7_error(global_eid);
    }
    break;

    case 19:
    {
        ecall_test_malloc8_error(global_eid);
    }
    break;

    case 20:
    {
        char buffer[50];
        size_t size = 50;
        ecall_check_serialize_ok(global_eid, buffer, size);
    }
    break;

    case 21:
    {
        char buffer[50];
        size_t size = 50;
        ecall_check_serialize_error(global_eid, buffer, size);
    }
    break;

    case 22:
    {
        int func_id = 1;
        ecall_check_function_id_error(global_eid, &func_id);
    }
    break;

    case 23:
    {
        int func_id = 1;
        ecall_check_function_id_ok(global_eid, &func_id);
    }
    break;

    case 24:
    {
        InputBuffer1 buf;
        ecall_test_input_struct1_error(global_eid, &buf);
    }
    break;

    case 25:
    {
        InputBuffer1 buf;
        ecall_test_input_struct1_ok(global_eid, &buf);
    }
    break;

    case 26:
    {
        InputBuffer2 buf;
        size_t size = sizeof(InputBuffer2);
        ecall_test_input_union1_error(global_eid, &buf, size);
    }
    break;

    case 27:
    {
        InputBuffer2 buf;
        size_t size = sizeof(InputBuffer2);
        ecall_test_input_union1_ok(global_eid, &buf, size);
    }
    break;

    case 28:
    {
        int buffer[16];
        unsigned size = 16;
        ecall_copy_information_global_error(global_eid, buffer, size);
    }
    break;

    case 29:
    {
        int buffer[16];
        unsigned size = 16;
        ecall_copy_information_global_ok(global_eid, buffer, size);
    }
    break;

    case 30:
    {
        int buffer[16];
        unsigned size = 16;
        ecall_copy_information_heap_error(global_eid, buffer, size);
    }
    break;

    case 31:
    {
        int buffer[16];
        unsigned size = 16;
        ecall_copy_information_heap_ok(global_eid, buffer, size);
    }
    break;

    case 32:
    {
        ecall_test_allocate_trusted_buffer_ok(global_eid);
    }
    break;

    case 33:
    {
        ecall_null_pointer_derefence_error(global_eid);
    }
    break;

    case 34:
    {
        ecall_null_pointer_derefence_ok(global_eid);
    }
    break;

    case 35:
    {
        ecall_test_validate_pointer_error(global_eid);
    }
    break;

    case 36:
    {
    }
    break;

    default:
        ret_val = -1;
        break;
    }

    return ret_val;
}

/* Application entry */
int SGX_CDECL main(int argc, char *argv[])
{
    (void)(argc);
    (void)(argv);

    /* Initialize the enclave */
    if (initialize_enclave() < 0)
    {
        printf("Enclave init fails. Enter a character before exit ...\n");
        getchar();
        return -1;
    }

    if (argc < 2)
    {
        print_test_cases(-1);
        return -1;
    }

    int test_case_id = atoi(argv[1]);
    if (test_case_id < 0 || test_case_id >= (int)(sizeof(sgx_test_case_list) / sizeof(sgx_test_case_list[0])))
    {
        printf("invalid test id number\n");
        return -1;
    }

    
    int ret_val = test_case_dispatch(test_case_id);

    /* Destroy the enclave */
    sgx_destroy_enclave(global_eid);

    return 0;
}
