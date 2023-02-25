ASM := nasm 
ASMFLAGS := -g
CC := i686-elf-gcc
CFLAGS := -Wall -Werror -O0 -nostdlib -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostartfiles -nodefaultlibs -Iinc
INCLUDES := -I./include

SRC_DIR := ./src
BIN_DIR := ./bin
BUILD_DIR := ./build

OS_BIN := ${BIN_DIR}/os.bin

BOOT_SRC := ${SRC_DIR}/boot/boot.asm
BOOT_BIN := ${BIN_DIR}/boot/boot.bin

KERNEL_C := $(wildcard ./src/*.c)
KERNEL_ASM := $(wildcard ./src/*.asm)

KERNEL_ASM_O := $(KERNEL_ASM:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.asm.o)
KERNEL_O := $(KERNEL_C:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
KERNEL_MERGED := ${BUILD_DIR}/kernel_merged.o
KERNEL_BIN := ${BIN_DIR}/kernel.bin



all: ${OS_BIN} 

${BOOT_BIN}: ${BOOT_SRC}
	mkdir -p $(dir $@)
	${ASM} ${ASMFLAGS} -f bin $< -o $@

${OS_BIN}: ${BOOT_BIN} ${KERNEL_BIN}
	rm -rf $@
	dd if=${BOOT_BIN} >> ${OS_BIN}
	dd if=${KERNEL_BIN} >> ${OS_BIN}
	#100 sectors of 512 bytes of zeroes
	dd if=/dev/zero bs=512 count=100 >> ${OS_BIN} 


${KERNEL_BIN}: ${KERNEL_ASM_O} ${KERNEL_O}
	#merge and link correct addresses to kernel files
	i686-elf-ld -g -relocatable $^ -o ${KERNEL_MERGED}
	${CC} ${CFLAGS} -T ./src/linker.ld -o $@ ${KERNEL_MERGED}

#Pattern Rules
${BUILD_DIR}/%.o: ${SRC_DIR}/%.c
	mkdir -p $(dir $@)
	$(CC) ${CFLAGS} ${INCLUDES} -c $< -o $@

${BUILD_DIR}/%.asm.o: ${SRC_DIR}/%.asm
	mkdir -p $(dir $@)
	${ASM} ${ASMFLAGS} -f elf $< -o $@


debugger: ${OS_BIN}
	cgdb -x ./debug/qemugdbinit 

dump-boot: ${BOOT_BIN}
	objdump -D -Mintel,i8086 -b binary -m i386 $<

dump: ${OS_BIN}
	objdump -D -b binary -m i386 $<

run: ${OS_BIN}
	qemu-system-x86_64 $< 

clean: 
	rm -rf ${BIN_DIR}/*
	rm -rf ${BUILD_DIR}/*

