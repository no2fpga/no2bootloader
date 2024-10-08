BOARD ?= icebreaker
CROSS ?= riscv-none-elf-
CC = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
ICEPROG = iceprog
DFU_UTIL = dfu-util

GITVER = $(shell git describe)
TARGET_BASE=no2bootloader-$(BOARD)
TARGET=$(TARGET_BASE)-$(GITVER)

BOARD_DEFINE=BOARD_$(shell echo $(BOARD) | tr a-z\- A-Z_)
CFLAGS=-Wall -Os -march=rv32i -mabi=ilp32 -ffreestanding -flto -nostartfiles -fomit-frame-pointer -Wl,--gc-section --specs=nano.specs -D$(BOARD_DEFINE) -I.

NO2USB_FW_VERSION=0
include ../gateware/cores/no2usb/fw/fw.mk
CFLAGS += $(INC_no2usb)

HEADERS_common=\
	config.h \
	console.h \
	led.h \
	mini-printf.h \
	spi.h \
	utils.h \
	$(HEADERS_no2usb)

SOURCES_common=\
	start.S \
	led.c \
	spi.c \
	utils.c \
	$(SOURCES_no2usb)

HEADERS_dfu=\
	usb_str_dfu.gen.h

SOURCES_dfu=\
	fw_dfu.c \
	usb_desc_dfu.c \
	$(NULL)

ifeq ($(ENABLE_UART),1)
SOURCES_common+= \
	console.c \
	mini-printf.c
else
SOURCES_common+= \
	console_dummy.c
endif

all: $(TARGET).bin $(TARGET_BASE).bin $(TARGET_BASE).elf


$(TARGET).elf: soc.lds $(HEADERS_dfu) $(SOURCES_dfu) $(HEADERS_common) $(SOURCES_common)
	$(CC) $(CFLAGS) -Wl,-Bstatic,-T,soc.lds,--strip-debug -o $@ $(SOURCES_common) $(SOURCES_dfu)


%.hex: %.bin
	./bin2hex.py $< $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

$(TARGET_BASE).bin: $(TARGET).bin
	ln -sf $< $@

$(TARGET_BASE).elf: $(TARGET).elf
	ln -sf $< $@

prog: $(TARGET).bin
	$(ICEPROG) -o 384k $<


clean:
	rm -f *.bin *.hex *.elf *.o *.gen.h

.PHONY: prog clean
