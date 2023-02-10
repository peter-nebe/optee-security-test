TC = /opt/toolchain/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
GCCFLAGS = -Wall -O2

all: armstub8-optee.bin

armstub8.o: armstub8.S
	$(TC)gcc -DGIC=1 -DBCM2711=1 -c $< -o $@

uart.o: uart.c
	$(TC)gcc $(GCCFLAGS) -c $< -o $@

armstub8-optee.elf: armstub8.o uart.o
	$(TC)ld -T link.ld -o $@

armstub8-optee.tmp: armstub8-optee.elf
	$(TC)objcopy $< -O binary $@

armstub8-optee.bin: armstub8-optee.tmp
	dd if=$< ibs=256 of=$@ conv=sync
