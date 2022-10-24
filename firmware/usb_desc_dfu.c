/*
 * usb_desc_dfu.c
 *
 * Copyright (C) 2019-2020 Sylvain Munaut
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <no2usb/usb_proto.h>
#include <no2usb/usb_dfu_proto.h>
#include <no2usb/usb_msos20.h>
#include <no2usb/usb.h>


static const struct {
	struct usb_conf_desc conf;
	struct usb_intf_desc if_fpga;
	struct usb_dfu_func_desc dfu_fpga;
	struct usb_intf_desc if_riscv;
	struct usb_dfu_func_desc dfu_riscv;
	struct usb_intf_desc if_bl_fpga;
	struct usb_dfu_func_desc dfu_bl_fpga;
	struct usb_intf_desc if_bl_riscv;
	struct usb_dfu_func_desc dfu_bl_riscv;
} __attribute__ ((packed)) _dfu_conf_desc = {
	.conf = {
		.bLength                = sizeof(struct usb_conf_desc),
		.bDescriptorType        = USB_DT_CONF,
		.wTotalLength           = sizeof(_dfu_conf_desc),
		.bNumInterfaces         = 1,
		.bConfigurationValue    = 1,
		.iConfiguration         = 4,
		.bmAttributes           = 0x80,
		.bMaxPower              = 0x32, /* 100 mA */
	},
	.if_fpga = {
		.bLength		= sizeof(struct usb_intf_desc),
		.bDescriptorType	= USB_DT_INTF,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 0,
		.bInterfaceClass	= 0xfe,
		.bInterfaceSubClass	= 0x01,
		.bInterfaceProtocol	= 0x02,
		.iInterface		= 5,
	},
	.dfu_fpga = {
		.bLength		= sizeof(struct usb_dfu_func_desc),
		.bDescriptorType	= USB_DFU_DT_FUNC,
		.bmAttributes		= 0x0f,
		.wDetachTimeOut		= 0,
		.wTransferSize		= 4096,
		.bcdDFUVersion		= 0x0101,
	},
	.if_riscv = {
		.bLength		= sizeof(struct usb_intf_desc),
		.bDescriptorType	= USB_DT_INTF,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 1,
		.bNumEndpoints		= 0,
		.bInterfaceClass	= 0xfe,
		.bInterfaceSubClass	= 0x01,
		.bInterfaceProtocol	= 0x02,
		.iInterface		= 6,
	},
	.dfu_riscv = {
		.bLength		= sizeof(struct usb_dfu_func_desc),
		.bDescriptorType	= USB_DFU_DT_FUNC,
		.bmAttributes		= 0x0f,
		.wDetachTimeOut		= 0,
		.wTransferSize		= 4096,
		.bcdDFUVersion		= 0x0101,
	},
	.if_bl_fpga = {
		.bLength		= sizeof(struct usb_intf_desc),
		.bDescriptorType	= USB_DT_INTF,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 2,
		.bNumEndpoints		= 0,
		.bInterfaceClass	= 0xfe,
		.bInterfaceSubClass	= 0x01,
		.bInterfaceProtocol	= 0x02,
		.iInterface		= 7,
	},
	.dfu_bl_fpga = {
		.bLength		= sizeof(struct usb_dfu_func_desc),
		.bDescriptorType	= USB_DFU_DT_FUNC,
		.bmAttributes		= 0x0f,
		.wDetachTimeOut		= 0,
		.wTransferSize		= 4096,
		.bcdDFUVersion		= 0x0101,
	},
	.if_bl_riscv = {
		.bLength		= sizeof(struct usb_intf_desc),
		.bDescriptorType	= USB_DT_INTF,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 3,
		.bNumEndpoints		= 0,
		.bInterfaceClass	= 0xfe,
		.bInterfaceSubClass	= 0x01,
		.bInterfaceProtocol	= 0x02,
		.iInterface		= 8,
	},
	.dfu_bl_riscv = {
		.bLength		= sizeof(struct usb_dfu_func_desc),
		.bDescriptorType	= USB_DFU_DT_FUNC,
		.bmAttributes		= 0x0f,
		.wDetachTimeOut		= 0,
		.wTransferSize		= 4096,
		.bcdDFUVersion		= 0x0101,
	},
};

static const struct usb_conf_desc * const _conf_desc_array[] = {
	&_dfu_conf_desc.conf,
};

static const struct usb_dev_desc _dev_desc = {
	.bLength		= sizeof(struct usb_dev_desc),
	.bDescriptorType	= USB_DT_DEV,
	.bcdUSB			= 0x0201,
	.bDeviceClass		= 0,
	.bDeviceSubClass	= 0,
	.bDeviceProtocol	= 0,
	.bMaxPacketSize0	= 64,
#if defined(BOARD_ICE1USB)
	.idVendor		= 0x1d50,
	.idProduct		= 0x6144,
#elif defined(BOARD_ICEPICK)
	.idVendor		= 0x1d50,
	.idProduct		= 0x6148,
#elif defined(BOARD_E1TRACER)
	.idVendor		= 0x1d50,
	.idProduct		= 0x6150,
#elif defined(BOARD_REDIP_SID)
	.idVendor		= 0x1d50,
	.idProduct		= 0x6156,
#elif defined(BOARD_ICE40_USBTRACE)
	.idVendor		= 0x1d50,
	.idProduct		= 0x617d,
#else
	.idVendor		= 0x1d50,
	.idProduct		= 0x6146,
#endif
	.bcdDevice		= 0x0006,	/* v0.6 */
	.iManufacturer		= 2,
	.iProduct		= 3,
	.iSerialNumber		= 1,
	.bNumConfigurations	= num_elem(_conf_desc_array),
};

#include "usb_str_dfu.gen.h"

const struct usb_stack_descriptors dfu_stack_desc = {
	.dev    = &_dev_desc,
	.bos    = &msos20_winusb_bos,
	.conf   = _conf_desc_array,
	.n_conf = num_elem(_conf_desc_array),
	.str    = _str_desc_array,
	.n_str  = num_elem(_str_desc_array),
};
