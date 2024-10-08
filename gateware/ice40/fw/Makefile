CROSS ?= riscv-none-elf-
CC = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
CFLAGS=-Wall -Os -march=rv32i -mabi=ilp32 -ffreestanding -nostartfiles


all: boot.hex


boot.elf: boot.lds boot.S
	$(CC) $(CFLAGS) -Wl,-Bstatic,-T,boot.lds,--strip-debug -DAPP_FLASH_ADDR=0x00060000 -o $@ boot.S


%.hex: %.bin
	../../../firmware/bin2hex.py $< $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@


clean:
	rm -f *.bin *.hex *.elf *.o *.gen.h

.PHONY: clean
