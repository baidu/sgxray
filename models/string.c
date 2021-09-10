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

#include <errno.h>
#include <smack.h>
#include <stdlib.h>
#include <string.h>

int memcmp(const void *str1, const void *str2, size_t n) {
  const unsigned char *s1 = (const unsigned char *)str1;
  const unsigned char *s2 = (const unsigned char *)str2;

  while (n--) {
    if (*s1 != *s2) {
      int result = __VERIFIER_nondet_int();
      if (*s1 < *s2) {
        __VERIFIER_assume(result < 0);
      } else {
        __VERIFIER_assume(result > 0);
      }
      return result;
    }
    s1++;
    s2++;
  }

  return 0;
}

char *strcpy(char *dest, const char *src) {
  char *save = dest;
  while ((*dest++ = *src++))
    ;
  return save;
}

char *strncpy(char *dest, const char *src, size_t n) {
  size_t i;

  for (i = 0; i < n && src[i] != '\0'; i++)
    dest[i] = src[i];
  for (; i < n; i++)
    dest[i] = '\0';

  return dest;
}

size_t strlen(const char *str) {
  size_t count = 0;
  while (str[count])
    count++;
  return count;
}

// comparison

int strcmp(const char *s1, const char *s2) {
  size_t n;
  for (n = 0; s1[n] == s2[n]; n++)
    if (s1[n] == '\0')
      return 0;
  return s1[n] - s2[n];
}

int strncmp(const char *s1, const char *s2, size_t n) {
  while (n--) {
    if (*s1 != *s2)
      return *s1 - *s2;
    s1++;
    s2++;
  }

  return 0;
}

// concatenation

char *strcat(char *dest, const char *src) {
  char *retDest = dest;

  while (*dest)
    dest++;
  while ((*dest++ = *src++))
    ;

  return retDest;
}

char *strncat(char *dest, const char *src, size_t n) {
  char *retDest = dest;

  while (*dest)
    dest++;
  while (n--)
    *dest++ = *src++;
  *dest = '\0';

  return retDest;
}

// searching

char *strchr(const char *src, int c) {
  while (*src != 0) {
    if (*src == c) {
      return (char *)src;
    }
    src++;
  }

  return (char *)0;
}

char *strrchr(const char *src, int c) {
  char *result = (char *)0;

  while (*src != 0) {
    if (*src == c) {
      result = (char *)src;
    }
    src++;
  }
  return result;
}

size_t strspn(const char *s1, const char *s2) {
  size_t n;
  const char *p;
  for (n = 0; *s1; s1++, n++) {
    for (p = s2; *p && *p != *s1; p++)
      ;
    if (!*p)
      break;
  }
  return n;
}

size_t strcspn(const char *s1, const char *s2) {
  size_t n;
  const char *p;
  for (n = 0; *s1; s1++, n++) {
    for (p = s2; *p && *p != *s1; p++)
      ;
    if (*p)
      break;
  }
  return n;
}

char *strpbrk(const char *s1, const char *s2) {
  for (char *c1 = (char *)s1; *c1; c1++)
    for (char *c2 = (char *)s2; *c2; c2++)
      if (*c1 == *c2)
        return c1;
  return 0;
}

char *strstr(const char *haystack, const char *needle) {
  for (; *haystack; haystack++) {
    const char *h, *n;
    for (h = haystack, n = needle; *h && *n && (*h == *n); h++, n++)
      ;
    if (*n == '\0')
      return (char *)haystack;
  }
  return 0;
}

static char *olds;
char *strtok(char *str, const char *delim) {
  if (!str)
    str = olds;

  // if str and olds are empty, return 0
  if (!str)
    return 0;

  // skip first delims
  str += strspn(str, delim);
  if (*str == '\0')
    return 0;

  char *tok = str;
  // find end of token
  str = strpbrk(str, delim);
  if (!str) // this token finishes the string
    olds = 0;
  else {
    *str = '\0';
    olds = str + 1;
  }
  return tok;
}

char *error_str = "xx";
char *strerror(int errnum) {
  error_str[0] = __VERIFIER_nondet_char();
  error_str[1] = __VERIFIER_nondet_char();
  return error_str;
}

// SGX memory management functions
void *memcpy(void *dst, const void *src, size_t n) {
  memcpy(dst, src, n);
  return dst;
}

void *memset(void *dst, int c, size_t n) {
  memset(dst, c, n);
  return dst;
}
//
// void *__memset(void *dst, int c, size_t n) {
//  memset(dst, c, n);
//  return dst;
//}
// Replication of
// https://github.com/intel/linux-sgx/blob/master/sdk/tsafecrt/pal/src/safecrt/memcpy_s.c
#ifndef _VALIDATE_RETURN_ERRCODE
#define _VALIDATE_RETURN_ERRCODE(c, e)                                         \
  if (!(c))                                                                    \
  return e
#endif

int memcpy_s(void *dst, size_t sizeInBytes, const void *src, size_t count) {
  if (count == 0) {
    /* nothing to do */
    return 0;
  }

  /* validation section */
  _VALIDATE_RETURN_ERRCODE(dst != NULL, EINVAL);
  if (src == NULL || sizeInBytes < count) {
    /* zeroes the destination buffer */
    memset(dst, 0, sizeInBytes);

    _VALIDATE_RETURN_ERRCODE(src != NULL, EINVAL);
    _VALIDATE_RETURN_ERRCODE(sizeInBytes >= count, ERANGE);
    /* useless, but prefast is confused */
    return EINVAL;
  }

  memcpy(dst, src, count);
  return 0;
}

// Replication of
// https://github.com/intel/linux-sgx/blob/master/common/src/sgx_memset_s.cpp
int memset_s(void *s, size_t smax, int c, size_t n) {
  int err = 0;

  if (s == NULL) {
    err = EINVAL;
    goto out;
  }

  if (n > smax) {
    err = EOVERFLOW;
    n = smax;
  }

  /* Calling through a volatile pointer should never be optimised away. */
  memset(s, c, n);

out:
  if (err == 0)
    return 0;
  else {
    errno = err;
    /* XXX call runtime-constraint handler */
    return err;
  }
}
