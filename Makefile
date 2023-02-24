ASM := nasm 

SRC_DIR := ./src
BIN_DIR := ./bin

BOOT_SRC := ${SRC_DIR}/boot/boot.asm
BOOT_BIN := ${BIN_DIR}/boot/boot.bin

KERNEL_SRC := ${SRC_DIR}/kernel.asm
KERNEL_BIN := ${BIN_DIR}/kernel.asm.o

OS_BIN := ${BIN_DIR}/os.bin

all: ${BOOT_BIN} 
	dd if=${BOOT_BIN} >> ${OS_BIN}

${BOOT_BIN}: ${BOOT_SRC}
	mkdir -p $(dir $@)
	${ASM} -g -f bin $< -o $@

${KERNEL_BIN}: ${KERNEL_SRC}
	${ASM} -g -f elf $< -o $@

bootloader-debug: ${BOOT}
	cgdb -x ./debug/qemugdbinit 

bootloader-dump: ${BOOT}
	objdump -D -Mintel,i8086 -b binary -m i386 $<

run: ${BOOT_BIN}
	qemu-system-x86_64 $< 

clean: 
	rm -rf ${BOOT_BIN} ${KERNEL_BIN}

