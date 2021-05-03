#!/usr/bin/env python3

import sys

import usb.core


class NO2Bootloader:

	POLL = 0.010	# 10 ms

	def __init__(self, vid=0x1d50, pid=0x6146):

		self.dev = usb.core.find(idVendor=vid, idProduct=pid)
		if self.dev is None:
			raise RuntimeError('Device not found')

		self.dev.set_configuration()

		if self.get_version() != (1, 0):
			raise RuntimeError('Unknown version')

	def get_version(self):
		resp = self.dev.ctrl_transfer(
			0xc1,	# bmRequestType
			0,		# bRequest,
			0,		# wValue=0,
			0,		# wIndex=0,
			2,		# data_or_wLength=None,
			None	# timeout=None,
		)
		return ( resp[0], resp[1] )

	def spi_exec(self, cmd, rlen=0):
		# Execute command
		buf = cmd + (b'\x00' * rlen)
		self.dev.ctrl_transfer(
			0x41,	# bmRequestType
			1,		# bRequest,
			0,		# wValue=0,
			0,		# wIndex=0,
			buf,	# data_or_wLength=None,
			None	# timeout=None,
		)

		# Get result
		buf = self.dev.ctrl_transfer(
			0xc1,		# bmRequestType
			2,			# bRequest,
			0,			# wValue=0,
			0,			# wIndex=0,
			len(buf),	# data_or_wLength=None,
			None		# timeout=None,
		)

		return bytes(buf[len(cmd):])


	def flash_busy(self):
		return bool(self.spi_exec(b'\x05', 1)[0] & 1)

	def flash_erase_4k(self, addr):
		# Write enable
		self.spi_exec(b'\x06')

		# Erase 4k
		self.spi_exec(b'\x20' + addr.to_bytes(3, 'big'))

		# Wait until flash is ready
		while self.flash_busy():
			pass

	def flash_program_page(self, addr, data):
		# Write enable
		self.spi_exec(b'\x06')

		# Write page
		self.spi_exec(b'\x02' + addr.to_bytes(3, 'big') + data)

		# Wait until flash is ready
		while self.flash_busy():
			pass

	def flash_read(self, addr, l):
		return self.spi_exec(b'\x03' + addr.to_bytes(3, 'big'), l)
