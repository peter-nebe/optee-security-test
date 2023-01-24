# optee-security-test
Testing the security of an OP-TEE installation

### Dependencies
- [OP-TEE](https://github.com/OP-TEE)
- [Cross compiler for ARM](https://developer.arm.com/-/media/Files/downloads/gnu-a/10.2-2020.11/binrel/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf.tar.xz)

### Overview
**optee-security-test** tests the behavior of an OP-TEE client when trying to run a trusted application (TA) signed with an incorrect key. The TA must not be executed in this case and the client API is expected to return a specific error.

### Details
*test-app* is an OP-TEE client application that is called in the REE. It in turn invokes a TA (*mock-ta*) in the TEE. *mock-ta* receives a value from the *test-app*, just multiplies it by a constant, and returns it. The *mock-ta* should work correctly because it was signed with the correct key that matches the OP-TEE OS.

*fake-ta* contains the same program code as *mock-ta* and has the same UUID. The only difference is that it was signed with a key that doesn't match the OP-TEE OS. The *test-app* only calls the TA using the UUID and doesn't care about signatures. The OP-TEE OS must recognize the invalid signature of the *fake-ta*, refuse execution of the *fake-ta* and return the error [TEE_ERROR_SECURITY](https://github.com/OP-TEE/optee_os/blob/8e74d47616a20eaa23ca692f4bbbf917a236ed94/lib/libutee/include/tee_api_defines.h#L126), which is then forwarded to the *test-app*.

The *qemu-run-test* script runs the test in the QEMU emulator. It calls the *test-app* three times and always copies *mock-ta* or *fake-ta* to the TA folder of the file system beforehand. The second time (with *fake-ta*) it expects the error code TEE_ERROR_SECURITY (0xffff000f).

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

#### Abbreviations
**TEE** - Trusted Execution Environment  
**REE** - Rich Execution Environment  
**TA** - Trusted Application  
**OP-TEE** - Open Portable TEE  
