#!/usr/bin/env python3

import sys

from no2bootloader import NO2Bootloader


def main(argv0, fn, addr=0):

	addr = int(addr, 0)
	if addr & 4095:
		raise RuntimeError('Address must be sector aligned !')

	bl = NO2Bootloader()

	with open(fn, 'rb') as fh:
		data = fh.read()

	for ofs in range(0, len(data), 256):
		if ofs & 4095 == 0:
			print(f"Erasing @0x{addr+ofs:08x}", file=sys.stderr)
			bl.flash_erase_4k(addr + ofs)

		chunk = data[ofs:ofs+256]
		if len(chunk) < 256:
			chunk = chunk + b'\x00' * (256 - len(chunk))

		print(f"Programming @0x{addr+ofs:08x}", file=sys.stderr)
		bl.flash_program_page(addr + ofs, chunk)

	return 0


if __name__ == '__main__':
	sys.exit(main(*sys.argv) or 0)
