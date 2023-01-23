# optee-security-test
Testing the security of an OP-TEE installation

### Dependencies
- [OP-TEE](https://github.com/OP-TEE)
- [Cross compiler for ARM](https://developer.arm.com/-/media/Files/downloads/gnu-a/10.2-2020.11/binrel/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf.tar.xz)

### Overview
**optee-security-test** tests the behavior of an OP-TEE client when trying to run a trusted application (TA) signed with an incorrect key. The TA must not be executed in this case and the client API is expected to return a specific error.

#### Successful test run
```
# cd /mnt/host
# ./qemu-run-test
call whith: mock-ta
value after invocation: 1230
test result: passed

call whith: fake-ta
TEEC_OpenSession failed with code 0xffff000f origin 0x3
test result: passed

call whith: mock-ta
value after invocation: 1230
test result: passed

overall test result: passed
#
```
