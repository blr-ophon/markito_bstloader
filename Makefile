ASM := nasm 
FLAGS := -g -f bin

SRC_DIR := ./src
BIN_DIR := ./bin

BOOT_SRC := ${SRC_DIR}/boot/boot.asm
BOOT_BIN := ${BIN_DIR}/boot/boot.bin

bootloader: ${BOOT_BIN} 

${BOOT_BIN}: ${BOOT_SRC}
	mkdir -p $(dir $@)
	${ASM} ${FLAGS} $< -o $@

bootloader-debug: ${BOOT}
	cgdb -x ./debug/qemugdbinit 

bootloader-dump: ${BOOT}
	objdump -D -Mintel,i8086 -b binary -m i386 $<

run: ${BOOT_BIN}
	qemu-system-x86_64 $< 

clean: 
	rm -rf ${BIN_DIR}/*

