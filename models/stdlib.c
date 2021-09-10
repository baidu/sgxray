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

//
// This file is distributed under the MIT License. See LICENSE for details.
//

#include <smack.h>
#include <stdlib.h>
#include <string.h>

void exit(int x) {
#if MEMORY_SAFETY
  __SMACK_code("assert {:valid_memtrack} $allocatedCounter == 0;");
#endif
  __SMACK_code("assume false;");
  while (1)
    ;
}

void abort(void) {
#if MEMORY_SAFETY
  __SMACK_code("assert {:valid_memtrack} $allocatedCounter == 0;");
#endif
  __SMACK_code("assume false;");
  while (1)
    ;
}

void *calloc(size_t num, size_t size) {
  void *ret;
  // if (__VERIFIER_nondet_int()) {
  //  ret = 0;
  //} else {
  //  __VERIFIER_atomic_begin();
  ret = malloc(num * size);
  memset(ret, 0, num * size);
  //  __VERIFIER_atomic_end();
  //}
  return ret;
}

void *realloc(void *ptr, size_t size) {
  void *ret;
  __VERIFIER_atomic_begin();
  free(ptr);
  ret = malloc(size);
  __VERIFIER_atomic_end();
  return ret;
}

long int strtol(const char *nptr, char **endptr, int base) {
  if (endptr != 0) {
    size_t size = strlen(nptr);
    unsigned nondet = __VERIFIER_nondet_unsigned_int();
    __VERIFIER_assume(nondet <= size);
    *endptr = (char *)nptr + nondet;
  }
  return __VERIFIER_nondet_long();
}

unsigned long int strtoul(const char *nptr, char **endptr, int base) {
  if (endptr != 0) {
    size_t size = strlen(nptr);
    unsigned nondet = __VERIFIER_nondet_unsigned_int();
    __VERIFIER_assume(nondet <= size);
    *endptr = (char *)nptr + nondet;
  }
  return __VERIFIER_nondet_unsigned_long();
}

unsigned long long int strtoull(const char *nptr, char **endptr, int base) {
  if (endptr != 0) {
    size_t size = strlen(nptr);
    unsigned nondet = __VERIFIER_nondet_unsigned_int();
    __VERIFIER_assume(nondet <= size);
    *endptr = (char *)nptr + nondet;
  }
  return __VERIFIER_nondet_unsigned_long_long();
}

double strtod(const char *nptr, char **endptr) {
  if (endptr != 0) {
    size_t size = strlen(nptr);
    unsigned nondet = __VERIFIER_nondet_unsigned_int();
    __VERIFIER_assume(nondet <= size);
    *endptr = (char *)nptr + nondet;
  }
  return __VERIFIER_nondet_double();
}

char *env_value_str = "xx";
char *getenv(const char *name) {
  if (__VERIFIER_nondet_int()) {
    return 0;
  } else {
    env_value_str[0] = __VERIFIER_nondet_char();
    env_value_str[1] = __VERIFIER_nondet_char();
    return env_value_str;
  }
}
