#ifndef _ENCLAVE_H_
#define _ENCLAVE_H_

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h> /* vsnprintf */
#include <string.h>
#include "sgx_trts.h"
#include "Enclave_t.h" /* print_string */

#include "user_types.h"


#if defined(__cplusplus)
extern "C" {
#endif

int printf(const char *fmt, ...);


#if defined(__cplusplus)
}
#endif

#endif /* !_ENCLAVE_H_ */
