TC = /opt/toolchain/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
GCCFLAGS = -Wall -O2

all: armstub8-uart-write.bin

armstub8.o: armstub8.S
	$(TC)gcc -DGIC=1 -DBCM2711=1 -c $< -o $@

uart-write.o: uart-write.c
	$(TC)gcc $(GCCFLAGS) -c $< -o $@

armstub8-uart-write.elf: armstub8.o uart-write.o
	$(TC)ld -T link.ld $+ -o $@

armstub8-uart-write.tmp: armstub8-uart-write.elf
	$(TC)objcopy $< -O binary $@

armstub8-uart-write.bin: armstub8-uart-write.tmp
	dd if=$< ibs=256 of=$@ conv=sync
