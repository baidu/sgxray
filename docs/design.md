## Design of SGXRay

### Overview

SGXRay is based on SMACK which mainly consists of a core translator `llvm2bpl` and SMACK libraries in C.
Currently, we only distribute a binary for `llvm2bpl` so our modifications are not visible. However, the source code
of our modifications of SMACK libraries are included in this repo. These libraries contain the memory model for SGX as
well as models for SGX APIs such as `sgx_is_within_enclave`, `sgx_is_outisde_enclave`, and `sgx_ocalloc`.

#### SGX Memory Model

Please refer to our upcoming paper for the details of SGX's memory model.
Our memory model is implemented at the Boogie level through SMACK's C-Boogie interoperation APIs.
The simple documentation of these APIs can be found following this [link](https://github.com/smackers/smack/blob/master/docs/boogie-code.md).
For their usages in SGX-Ray, please refer to `$$alloc` and `$sgx_ocalloc` procedure in [smack.c](../models/smack.c).

### Add Model for New APIs

Please add new APIs in [sgx.c](../models/sgx.c). You can refer to the existing models in this file.
The general rule to add new APIs is to reuse SDK source code as much as possible. Namely, add bc files of
SDK lib via SMACK option `--sgx-sdk-libs`. If the model of an API requires significantly rewriting (e.g., there's a loop
with large iterations), we should put the implementation in the file.
