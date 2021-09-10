/*
 * Copyright 2021 Baidu 
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/*
 * Copyright (C) 2011-2021 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "smack.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Intel SGX SDK error codes
//
#define SGX_ERROR_INVALID_PARAMETER 2
#define SGX_ERROR_UNEXPECTED 1
#define SGX_SUCCESS 0

// sgx_is_within_enclave()
// Parameters:
//      addr - the start address of the buffer
//      size - the size of the buffer
// Return Value:
//      1 - the buffer is strictly within the enclave
//      0 - the buffer is not strictly within the enclave
//
// SGX-Ray's model for Intel SGX SDK's sgx_is_within_enclave function
int sgx_is_within_enclave(const void *addr, size_t size) {
  int ret = __VERIFIER_nondet_int();
#ifdef BIT_PRECISE
  __SMACK_code("@ := if (($sge.ref.bool(@, $ENCLAVE_BASE) &&"
               " $slt.ref.bool($add.ref(@, $i2p.bv64.ref(@)), $MALLOC_TOP)) ||"
               " ($sge.ref.bool(@, $GLOBALS_BOTTOM) &&"
               " $slt.ref.bool($add.ref(@, $i2p.bv64.ref(@)), $0.ref)))"
               " then @i else @i;",
               ret, addr, addr, size, addr, addr, size, 1, 0);
#else
  __SMACK_code("@ := if (($sge.ref.bool(@, $ENCLAVE_BASE) &&"
               " $slt.ref.bool($add.ref(@, $i2p.i64.ref(@)), $MALLOC_TOP)) ||"
               " ($sge.ref.bool(@, $GLOBALS_BOTTOM) &&"
               " $slt.ref.bool($add.ref(@, $i2p.i64.ref(@)), $0.ref)))"
               " then @i else @i;",
               ret, addr, addr, size, addr, addr, size, 1, 0);
#endif
  return ret;
}

// sgx_is_outside_enclave()
// Parameters:
//      addr - the start address of the buffer
//      size - the size of the buffer
// Return Value:
//      1 - the buffer is strictly outside the enclave
//      0 - the buffer is not strictly outside the enclave
//
// SGX-Ray's model for Intel SGX SDK's sgx_is_within_enclave function
int sgx_is_outside_enclave(const void *addr, size_t size) {
  int ret = __VERIFIER_nondet_int();
#ifdef BIT_PRECISE
  __SMACK_code("@ := if ($sge.ref.bool(@, $0.ref) &&"
               " $slt.ref.bool($add.ref($i2p.bv64.ref(@), @), $ENCLAVE_BASE))"
               " then @i else @i;",
               ret, addr, size, addr, 1, 0);
#else
  __SMACK_code("@ := if ($sge.ref.bool(@, $0.ref) &&"
               " $slt.ref.bool($add.ref($i2p.i64.ref(@), @), $ENCLAVE_BASE))"
               " then @i else @i;",
               ret, addr, size, addr, 1, 0);
#endif
  return ret;
}

// sgx_ocalloc()
// Parameters:
//      size - bytes to allocate on the outside stack
// Return Value:
//      the pointer to the allocated space on the outside stack
//      NULL - fail to allocate
//
// SGX-Ray's model for Intel SGX SDK's sgx_ocalloc function
void *sgx_ocalloc(size_t size) {
  void *ret = __VERIFIER_nondet_pointer();
#ifdef BIT_PRECISE
  __SMACK_code("call @ := $sgx_ocalloc($i2p.bv64.ref(@));", ret, size);
#else
  __SMACK_code("call @ := $sgx_ocalloc($i2p.i64.ref(@));", ret, size);
#endif
  return ret;
}

// sgx_ocfree()
// Parameters:
//      N/A
// Return Value:
//      N/A
//
// SGX-Ray's model for Intel SGX SDK's sgx_ocfree function
// TODO: We need to model this function eventually otherwise
// we can miss bugs if a freed pointer is still used
// inside an enclave.
void sgx_ocfree() { __SMACK_code("// Warning: sgx_ocfree ignored!"); }

// __SMACK_init_func_enclave()
// Parameters:
//      N/A
// Return Value:
//      N/A
//
// Initialize the allocation start address inside enclaves
__SMACK_INIT(enclave) {
  __SMACK_code("$EnclaveCurrAddr := $add.ref($ENCLAVE_BASE, $1024.ref);");
}

// sgx_read_rand()
// Parameters:
//      rand - buffer to hold randomized bytes
//      length_in_bytes - requested size
// Return Value:
//      status
//
// SGX-Ray's model for Intel SGX SDK's sgx_read_rand function
// modified from https://github.com/intel/linux-sgx/blob/edfe42a517b3e4b1d81204c3cdef6da6cb35fefc/sdk/trts/trts.cpp#L272
int sgx_read_rand(unsigned char *rand, size_t length_in_bytes) {
  // check parameters
  //
  // rand can be within or outside the enclave
  if (!rand || !length_in_bytes) {
    return SGX_ERROR_INVALID_PARAMETER;
  }
  if (!sgx_is_within_enclave(rand, length_in_bytes) &&
      !sgx_is_outside_enclave(rand, length_in_bytes)) {
    return SGX_ERROR_INVALID_PARAMETER;
  }
  // changed the loop in Intel SGX SDK's code into malloc and
  // memcpy to avoid loop bound issues
  unsigned char *tmp = (unsigned char *)malloc(length_in_bytes);
  if (!tmp)
    return SGX_ERROR_UNEXPECTED;
  memcpy(rand, tmp, length_in_bytes);
  // underapproximate return status: it could be a failing status
  return SGX_SUCCESS;
}

// oe_allocate_ocall_buffer()
// Parameters:
//      size - bytes to allocate outside an enclave
// Return Value:
//      the pointer to the allocated space outside an enclave
//
// SGX-Ray's model for openenclave SDK's sgx_allocate_ocall_buffer function
// Note that internally SMACK doesn't distinguish stack vs heap allocation
// so we can use sgx_ocalloc to model this function.
void *oe_allocate_ocall_buffer(size_t size) { return sgx_ocalloc(size); }

// oe_is_within_enclave()
// Parameters:
//      p - the start address of the buffer
//      n - the size of the buffer
// Return Value:
//      1 - the buffer is strictly within the enclave
//      0 - the buffer is not strictly within the enclave or is NULL
//
// SGX-Ray's model for openenclave's oe_is_within_enclave function
bool oe_is_within_enclave(const void *p, size_t n) {
  if (!p)
    return false;

  return sgx_is_within_enclave(p, n);
}

// oe_is_outside_enclave()
// Parameters:
//      p - the start address of the buffer
//      n - the size of the buffer
// Return Value:
//      1 - the buffer is strictly outside the enclave
//      0 - the buffer is not strictly outside the enclave or is NULL
//
// SGX-Ray's model for openenclave's oe_is_outside_enclave function
bool oe_is_outside_enclave(const void *p, size_t n) {
  if (!p)
    return false;

  return sgx_is_outside_enclave(p, n);
}

// oe_malloc()
// Parameters:
//      n - the size of the requested memory
// Return Value:
//      returns a dynamically alloated buffer inside an enclave
//
// SGX-Ray's model for openenclave's oe_malloc function
void *oe_malloc(size_t n) { return malloc(n); }

// oe_realloc()
// Parameters:
//      ptr  - the buffer to resize
//      size - the new buffer size
// Return Value:
//      returns a resized dynamically alloated buffer inside an enclave
//
// SGX-Ray's model for openenclave's oe_realloc function
void *oe_realloc(void *ptr, size_t size) { return realloc(ptr, size); }

// oe_realloc()
// Parameters:
//      nmemb - the number of objects
//      size  - the size of an object
// Return Value:
//      returns a calloced buffer inside an enclave
//
// SGX-Ray's model for openenclave's oe_calloc function
void *oe_calloc(size_t nmemb, size_t size) { return calloc(nmemb, size); }

// oe_abort()
// Parameters:
//      N/A
// Return Value:
//      N/A
//
// SGX-Ray's model for openenclave's oe_abort function
void oe_abort(void) { abort(); }
