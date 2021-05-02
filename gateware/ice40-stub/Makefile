# Project config
PROJ := ice40-stub

PROJ_DEPS := no2misc
PROJ_RTL_SRCS := $(addprefix rtl/, \
	flash_lock.v \
)
PROJ_TESTBENCHES := \
	flash_lock_tb
PROJ_TOP_SRC := rtl/top.v
PROJ_TOP_MOD := top

# Target config
BOARD ?= icebreaker
DEVICE := $(lastword $(shell grep '^## dev:' data/top-$(BOARD).pcf || echo up5k))
PACKAGE := $(lastword $(shell grep '^## pkg:' data/top-$(BOARD).pcf || echo sg48))

NEXTPNR_ARGS = --freq 48 --no-promote-globals

# Include default rules
include ../build/project-rules.mk

# Custom rules
	# Just bootstub in multi-boot image
$(BUILD_TMP)/boot_mb.bin: $(BUILD_TMP)/$(PROJ).bin
	./sw/mkmultiboot.py $@ $<

build-mb: $(BUILD_TMP)/boot_mb.bin

prog-mb: $(BUILD_TMP)/boot_mb.bin
	$(ICEPROG) $<

sudo-prog-mb: $(BUILD_TMP)/boot_mb.bin
	@echo 'Executing prog as root!!!'
	sudo $(ICEPROG) $<

	# Full DFU bootloader image
GW_PROJ_BASE=$(realpath $(BUILD_TMP)/../../ice40)
FW_PROJ_BASE=$(realpath $(BUILD_TMP)/../../../firmware)

$(BUILD_TMP)/bootloader.bin: $(BUILD_TMP)/$(PROJ).bin $(GW_PROJ_BASE)/build-tmp/no2bootloader-ice40.bin $(FW_PROJ_BASE)/no2bootloader-$(BOARD).bin
	./sw/mkmultiboot.py $@ $(BUILD_TMP)/$(PROJ).bin $(GW_PROJ_BASE)/build-tmp/no2bootloader-ice40.bin:$(FW_PROJ_BASE)/no2bootloader-$(BOARD).bin

$(GW_PROJ_BASE)/build-tmp/no2bootloader-ice40.bin:
	make -C $(GW_PROJ_BASE)

$(FW_PROJ_BASE)/no2bootloader-$(BOARD).bin:
	make -C $(FW_PROJ_BASE) no2bootloader-$(BOARD).bin

bootloader-clean:
	if test "$(PRE_CLEAN)" = "1"; then \
		make clean; \
		make -C $(GW_PROJ_BASE) clean; \
		make -C $(FW_PROJ_BASE) clean; \
		mkdir -p $(BUILD_TMP); \
	fi

bootloader: bootloader-clean $(BUILD_TMP)/bootloader.bin

prog-bootloader: bootloader
	$(ICEPROG) $(BUILD_TMP)/bootloader.bin

sudo-prog-bootloader: bootloader
	@echo 'Executing prog as root!!!'
	sudo $(ICEPROG) $(BUILD_TMP)/bootloader.bin


.PHONY: build-mb prog-mb sudo-prog-mb bootloader-clean bootloader prog-bootloader sudo-prog-bootloader
