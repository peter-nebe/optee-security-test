# optee-security-test
Testing the security of an [OP-TEE](## "Open Portable TEE") installation

### Dependencies
- [OP-TEE](https://github.com/OP-TEE)
- [Cross compiler for ARM](https://developer.arm.com/downloads/-/gnu-a)

### Overview
**optee-security-test** tests the behavior of an OP-TEE client when trying to run a trusted application (TA) signed with an incorrect key. The TA must not be executed in this case and the client API is expected to return a specific error.

### Details
[test-app](test-app.cpp) is an OP-TEE client application that is called in the [REE](## "Rich Execution Environment"). It in turn invokes a [TA](## "Trusted Application") ([mock-ta](mock-ta.cpp)) in the [TEE](## "Trusted Execution Environment"). *mock-ta* receives a value from the *test-app*, just multiplies it by a constant, and returns it. The *mock-ta* should work correctly because it was signed with the correct key that matches the OP-TEE OS.

[fake-ta](mk-rpi4#L23) contains the same program code as *mock-ta* and has the same UUID. The only difference is that it was signed with a key that doesn't match the OP-TEE OS. The *test-app* only calls the TA using the UUID and doesn't care about signatures. The OP-TEE OS must recognize the invalid signature of the *fake-ta*, refuse execution of the *fake-ta* and return the error [TEE_ERROR_SECURITY](https://github.com/OP-TEE/optee_os/blob/8e74d47616a20eaa23ca692f4bbbf917a236ed94/lib/libutee/include/tee_api_defines.h#L126), which is then forwarded to the *test-app*.

The [run-test](run-test) script runs the test on the target or in [QEMU](## "Quick Emulator"). It calls the *test-app* three times and always copies *mock-ta* or *fake-ta* to the TA folder of the file system beforehand. The second time (with *fake-ta*) it expects the error code TEE_ERROR_SECURITY (0xffff000f).

#### Successful test run
```
[peter@PC1 ~]$ ssh root@RPI1.localdomain
# ./run-test
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

### Platforms supported
So far the Raspberry Pi 4 and QEMU are supported. It shouldn't be difficult to adapt the setup to more platforms supported by OP-TEE. Each platform requires a suitably built OP-TEE OS and a special bootloader. Furthermore, the OP-TEE client library and a Linux kernel with OP-TEE driver are required.

#### Raspberry Pi 4
The Raspberry Pi 4 is not yet officially supported by OP-TEE, but the RPi3 is. So I provisionally adapted the RPi3 porting to the RPi4. A few extensions were also required in the ARM Trusted Firmware. To run **optee-security-test** on the RPi4, first build the forks according to these [building instructions](https://github.com/peter-nebe/optee_os#building-instructions).

Once you get OP-TEE working on the RPi4, build the apps with [mk-rpi4](mk-rpi4). Call [inst-rpi](inst-rpi) to load the apps onto the RPi. On the RPi, call *run-test* as shown above.

#### QEMU
A good guide to get OP-TEE working in QEMU can be found [here](https://kickstartembedded.com/2022/11/07/op-tee-part-3-setting-up-op-tee-on-qemu-raspberry-pi-3/). Once you've done that, build the apps with [mk-qemu](mk-qemu). Call [start-qemu](start-qemu). In QEMU, call *run-test* like this:
```
Welcome to Buildroot, type root or test to login
buildroot login: root
# cd /mnt/host/build/qemu
# ../../run-test
```
