/*
 * boards.vh
 *
 * vim: ts=4 sw=4 syntax=verilog
 *
 * Copyright (C) 2019-2020  Sylvain Munaut <tnt@246tNt.com>
 * SPDX-License-Identifier: CERN-OHL-P-2.0
 */

`ifdef BOARD_BITSY_V0
	// 1bitsquared iCEbreaker bitsy prototypes (v0.x)
	`define HAS_USB
	`define HAS_2LED
	`define HAS_RGB
	`define RGB_DIM 3
	`define RGB_MAP 12'h201		// 41=Blue, 40=Red, 39=Green
		// Assumes W25Q128
	`define FLASH_LOCK { \
		2'b01, 8'h50,   /* WRITE_ENABLE_VOLTATILE */ \
		2'b00, 8'h01,	/* WRITE_SR */ \
		2'b00, 8'h28,	/* SR1 value */ \
		2'b11, 8'h03	/* SR2 value */ \
	}
`elsif BOARD_BITSY_V1
	// 1bitsquared iCEbreaker bitsy prod (v1.x)
	`define HAS_USB
	`define HAS_2LED
	`define HAS_RGB
	`define RGB_DIM 3
	`define RGB_MAP 12'h210		// 41=Blue, 40=Green, 39=Red
		// Assumes W25Q128
	`define FLASH_LOCK { \
		2'b01, 8'h50,   /* WRITE_ENABLE_VOLTATILE */ \
		2'b00, 8'h01,	/* WRITE_SR */ \
		2'b00, 8'h28,	/* SR1 value */ \
		2'b11, 8'h03	/* SR2 value */ \
	}
`elsif BOARD_ICEBREAKER
	// 1bitsquare iCEbreaker
	`define HAS_USB
	`define HAS_2LED
	`define HAS_RGB
	`define RGB_DIM 3
	`define RGB_MAP 12'h012		// 41=Red, 40=Green, 39=Blue
//	`define RGB_MAP 12'h120		// 41=Green, 40=Blue, 39=Red (Hacked v1.0b)
		// Assumes W25Q128
	`define FLASH_LOCK { \
		2'b01, 8'h50,   /* WRITE_ENABLE_VOLTATILE */ \
		2'b00, 8'h01,	/* WRITE_SR */ \
		2'b00, 8'h28,	/* SR1 value */ \
		2'b11, 8'h03	/* SR2 value */ \
	}
`elsif BOARD_ICEPICK
	// iCEpick
	`define MISC_SEL 2'b01
	`define HAS_USB
	`define HAS_RGB
	`define RGB_MAP 12'h012		// 41=Red, 40=Green, 39=Blue
//	`define RGB_MAP 12'h210		// 41=Blue, 40=Green, 39=Red (Alt RGB LED)
		// Assumes W25Q80 or GD25Q16
	`define FLASH_LOCK { \
		2'b01, 8'h50,   /* WRITE_ENABLE_VOLTATILE */ \
		2'b00, 8'h01,	/* WRITE_SR */ \
		2'b00, 8'h30,	/* SR1 value */ \
		2'b11, 8'h01	/* SR2 value */ \
	}
`elsif BOARD_ICE1USB
	// icE1usb
	`define HAS_USB
	`define HAS_RGB
	`define RGB_MAP 12'h012		// 41=Red, 40=Green, 39=Blue
		// Assumes W25Q80
	`define FLASH_LOCK { \
		2'b01, 8'h50,   /* WRITE_ENABLE_VOLTATILE */ \
		2'b00, 8'h01,	/* WRITE_SR */ \
		2'b00, 8'h30,	/* SR1 value */ \
		2'b11, 8'h01	/* SR2 value */ \
	}
`elsif BOARD_E1TRACER
	// osmocom E1 tracer
	`define MISC_SEL 2'b01		// Compatibility with icepick proto
	`define HAS_USB
	`define HAS_RGB
	`define RGB_MAP 12'h012		// 41=Red, 40=Green, 39=Blue
		// Assumes W25Q80
	`define FLASH_LOCK { \
		2'b01, 8'h50,   /* WRITE_ENABLE_VOLTATILE */ \
		2'b00, 8'h01,	/* WRITE_SR */ \
		2'b00, 8'h30,	/* SR1 value */ \
		2'b11, 8'h01	/* SR2 value */ \
	}
`elsif BOARD_FOMU_HACKER
	// FOMU Hacker version
	`define HAS_USB
	`define HAS_RGB
	`define RGB_MAP 12'h012		// 41=Red, 40=Green, 39=Blue
		// Assumes AT25SF161
	`define FLASH_LOCK { \
		2'b01, 8'h50,   /* WRITE_ENABLE_VOLTATILE */ \
		2'b00, 8'h01,	/* WRITE_SR */ \
		2'b00, 8'h30,	/* SR1 value */ \
		2'b11, 8'h01	/* SR2 value */ \
	}
`elsif BOARD_FOMU_PVT1
	// FOMU PVT1 (prod version)
	`define HAS_USB
	`define HAS_RGB
	`define RGB_MAP 12'h012		// 41=Red, 40=Green, 39=Blue
		// Assumes GD25Q16C
	`define FLASH_LOCK { \
		2'b01, 8'h50,   /* WRITE_ENABLE_VOLTATILE */ \
		2'b00, 8'h01,	/* WRITE_SR */ \
		2'b00, 8'h30,	/* SR1 value */ \
		2'b11, 8'h03	/* SR2 value */ \
	}
		// For MX25R1635F: Don't flash lock, that flash sucks, only OTP stuff ...
`elsif BOARD_REDIP_SID
	// reDIP-SID
	`define HAS_USB
	`define HAS_1LED
		// Assumes W25Q128
	`define FLASH_LOCK { \
		2'b01, 8'h50,   /* WRITE_ENABLE_VOLTATILE */ \
		2'b00, 8'h01,	/* WRITE_SR */ \
		2'b00, 8'h28,	/* SR1 value */ \
		2'b11, 8'h03	/* SR2 value */ \
	}
`elsif BOARD_ICE40_USBTRACE
	// iCE40 USB trace ( https://gitea.osmocom.org/electronics/ice40-usbtrace )
	`define HAS_USB
	`define HAS_RGB
	`define RGB_DIM 3
	`define RGB_MAP 12'h210		// 41=Blue, 40=Green, 39=Red
		// Assumes W25Q80
	`define FLASH_LOCK { \
		2'b01, 8'h50,   /* WRITE_ENABLE_VOLTATILE */ \
		2'b00, 8'h01,	/* WRITE_SR */ \
		2'b00, 8'h30,	/* SR1 value */ \
		2'b11, 8'h01	/* SR2 value */ \
	}
`elsif BOARD_ICE_DONGLE
	// @emeb ice-dongle
	`define HAS_USB
	`define HAS_RGB
	`define RGB_DIM 3
	`define RGB_MAP 12'h210		// 41=Blue, 40=Green, 39=Red
		// Assumes W25Q64
	`define FLASH_LOCK { \
		2'b01, 8'h50,   /* WRITE_ENABLE_VOLTATILE */ \
		2'b00, 8'h01,	/* WRITE_SR */ \
		2'b00, 8'h28,	/* SR1 value */ \
		2'b11, 8'h03	/* SR2 value */ \
	}
`elsif BOARD_XMAS_SNOOPY
	// @tnt xmas-snoopy led controller
	`define HAS_USB
	`define MISC_SEL 2'b10
		// Assumes S25FL064L
	`define FLASH_LOCK { \
		2'b01, 8'h50,   /* WRITE_ENABLE_VOLTATILE */ \
		2'b00, 8'h01,	/* WRITE_SR */ \
		2'b00, 8'h2C,	/* SR1 value */ \
		2'b11, 8'h03	/* SR2 value */ \
	}
`endif

// Defaults
`ifndef RGB_CURRENT_MODE
`define RGB_CURRENT_MODE "0b1"
`endif

`ifndef RGB0_CURRENT
`define RGB0_CURRENT "0b000001"
`endif

`ifndef RGB1_CURRENT
`define RGB1_CURRENT "0b000001"
`endif

`ifndef RGB2_CURRENT
`define RGB2_CURRENT "0b000001"
`endif

`ifndef RGB_MAP
// [11:8] - Color of RGB2 / pin 41
// [ 7:0] - Color of RGB1 / pin 40
// [ 3:0] - Color of RGB0 / pin 39
//          0=Red 1=Green 2=Blue
`define RGB_MAP 12'h210		// 41=Blue, 40=Green, 39=Red
`endif
