/*
 * fw_dfu.c
 *
 * Copyright (C) 2019-2020 Sylvain Munaut
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "console.h"
#include "led.h"
#include "mini-printf.h"
#include "spi.h"
#include <no2usb/usb.h>
#include <no2usb/usb_dfu.h>
#include <no2usb/usb_msos20.h>
#include "utils.h"


extern const struct usb_stack_descriptors dfu_stack_desc;

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
	volatile uint32_t *boot = (void*)0x80000000;
	*boot = (1 << 2) | (2 << 0);
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

static const struct usb_dfu_zone dfu_zones[] = {
	{ 0x00080000, 0x000a0000 },     /* iCE40 bitstream */
	{ 0x000a0000, 0x000c0000 },     /* RISC-V firmware */
};


// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

void main()
{
	int cmd = 0;

	/* Init console IO */
	console_init();
	puts("Booting DFU image..\n");

	/* LED */
	led_init();
	led_color(72, 64, 0);
	led_blink(true, 150, 150);
	led_breathe(true, 50, 100);
	led_state(true);

	/* SPI */
	spi_init();

	/* Enable USB directly */
	serial_no_init();
	usb_init(&dfu_stack_desc);
	usb_dfu_init(dfu_zones, 2);
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
