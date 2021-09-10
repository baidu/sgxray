## Enclave Test Cases
There are two steps to run the tests. The first one is to get bc files.
The second one is to run the analysis.

### Build bc files
We recommand using the Docker container we provide for this task.

```bash
cd tests/intel_sgx_sdk
docker run --rm -v $(pwd):/sgx -w /sgx --user $UID -it shaobohe/sgx-ray-frontend-intel
```

Inside the Docker container, please run,

```bash
source /opt/smack-wllvm/default.environment
source /opt/sgxsdk/environment
make bc
```

### Run test cases

```bash
cd tests/intel_sgx_sdk
./scripts/run.py ./scripts/sample.yml
```

