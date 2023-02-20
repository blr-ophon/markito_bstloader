ASM := nasm 
FLAGS := -f bin

SRC := ./bloader.asm
BIN := $(SRC:%.asm=%.bin)

${BIN}: ${SRC}
	${ASM} ${FLAGS} $< -o $@

run: ${BIN}
	qemu-system-x86_64 $<

clean: 
	rm -rf ${BIN}

