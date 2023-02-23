ASM := nasm 
FLAGS := -g -f bin

SRC_DIR := ./src
BIN_DIR := ./bin

BLOADER := ${BIN_DIR}/bloader.bin

bootloader: ${BLOADER} 

${BIN_DIR}/%.bin: ${SRC_DIR}/%.asm
	mkdir -p $(dir $@)
	${ASM} ${FLAGS} $< -o $@

run: ${BLOADER}
	qemu-system-x86_64 $< 

debug: ${BLOADER}
	cgdb -x ./debug/qemugdbinit 

dump: ${BLOADER}
	objdump -D -Mintel,i8086 -b binary -m i386 $<

clean: 
	rm -rf ${BLOADER}

