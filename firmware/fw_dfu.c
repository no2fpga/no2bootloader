/*
 * fw_dfu.c
 *
 * Copyright (C) 2019-2020 Sylvain Munaut
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "config.h"
#include "console.h"
#include "led.h"
#include "mini-printf.h"
#include "spi.h"
#include <no2usb/usb.h>
#include <no2usb/usb_dfu.h>
#include <no2usb/usb_dfu_proto.h>
#include <no2usb/usb_msos20.h>
#include "utils.h"


extern const struct usb_stack_descriptors dfu_stack_desc;


struct wb_misc {
	uint32_t boot;
	uint32_t led;
} __attribute__((packed,aligned(4)));

static volatile struct wb_misc * const misc_regs = (void*)(MISC_BASE);


static void
serial_no_init()
{
	uint8_t buf[8];
	char *id, *desc;
	int i;

	flash_manuf_id(buf);
	printf("Flash Manufacturer : %s\n", hexstr(buf, 3, true));

	flash_unique_id(buf);
	printf("Flash Unique ID    : %s\n", hexstr(buf, 8, true));

	printf("Flash SR1 %02x / SR2 %02x\n", flash_read_sr(1), flash_read_sr(2));

	/* Overwrite descriptor string */
		/* In theory in rodata ... but nothing is ro here */
	id = hexstr(buf, 8, false);
	desc = (char*)dfu_stack_desc.str[1];
	for (i=0; i<16; i++)
		desc[2 + (i << 1)] = id[i];
}

static void
boot_app(void)
{
	/* Force re-enumeration */
	usb_disconnect();

	/* Boot firmware */
	misc_regs->boot = (1 << 2) | (2 << 0);
}

static void
patch_descriptors(bool bl_upgrade)
{
	volatile struct usb_conf_desc *conf = (void*)dfu_stack_desc.conf[0];
	int n = bl_upgrade ? 4 : 2;

	/* We patch the descriptor length ... in RO section but not really RO */
	conf->wTotalLength = sizeof( struct usb_conf_desc) + n * (sizeof(struct usb_intf_desc) + sizeof(struct usb_dfu_func_desc));
}

static void
set_single_led(bool bl_upgrade)
{
	/* Set the single led to slow or fast flash */
	misc_regs->led = (1 << 31) | ((bl_upgrade ? 120 : 600) << 16) | (120 << 0);
}


// ---------------------------------------------------------------------------
// USB DFU driver callbacks
// ---------------------------------------------------------------------------

void
usb_dfu_cb_reboot(void)
{
	boot_app();
}

bool
usb_dfu_cb_flash_busy(void)
{
	return flash_read_sr(1) & 1;
}

void
usb_dfu_cb_flash_erase(uint32_t addr, unsigned size)
{
	flash_write_enable();

	switch (size) {
	case 4096:  flash_sector_erase(addr);
	case 32678: flash_block_erase_32k(addr);
	case 65536: flash_block_erase_64k(addr);
	}
}

void
usb_dfu_cb_flash_program(const void *data, uint32_t addr, unsigned size)
{
	flash_write_enable();
	flash_page_program(data, addr, size);
}

void
usb_dfu_cb_flash_read(void *data, uint32_t addr, unsigned size)
{
	flash_read(data, addr, size);
}

void
usb_dfu_cb_flash_raw(void *data, unsigned len)
{
	struct spi_xfer_chunk sx[1] = {
		{ .data = data, .len = len, .read = true, .write = true, },
	};
	spi_xfer(SPI_CS_FLASH, sx, 1);
}


static const struct usb_dfu_zone dfu_zones[] = {
	{ 0x00080000, 0x000a0000 },     /* iCE40 bitstream */
	{ 0x000a0000, 0x000c0000 },     /* RISC-V firmware */
	{ 0x00040000, 0x00060000 },     /* Bootloader bitstream */
	{ 0x00060000, 0x00080000 },     /* Bootloader firmware  */
};


// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

void main()
{
	bool bl_upgrade = false;
	int cmd = 0;

	/* Init console IO */
	console_init();
	puts("Booting DFU image..\n");

	/* LED */
	led_init();
	led_color(8, 8, 8);
	led_blink(true, 150, 150);
	led_breathe(true, 50, 100);
	led_state(true);

	/* SPI */
	spi_init();

	/* Should be allow boot loader upgrad ? */
	bl_upgrade = ((flash_read_sr(1) & 0x7c) == 0);

	if (bl_upgrade)
		led_color(64, 0, 16);
	else
		led_color(0, 16, 64);

	set_single_led(bl_upgrade);
	patch_descriptors(bl_upgrade);

	/* Enable USB directly */
	serial_no_init();
	usb_init(&dfu_stack_desc);
	usb_dfu_init(dfu_zones, 4);
	usb_msos20_init(NULL);
	usb_connect();

	/* Main loop */
	while (1)
	{
		/* Prompt ? */
		if (cmd >= 0)
			printf("Command> ");

		/* Poll for command */
		cmd = getchar_nowait();

		if (cmd >= 0) {
			if (cmd > 32 && cmd < 127) {
				putchar(cmd);
				putchar('\r');
				putchar('\n');
			}

			switch (cmd)
			{
			case 'b':
				boot_app();
				break;
			default:
				break;
			}
		}

		/* USB poll */
		usb_poll();
	}
}
