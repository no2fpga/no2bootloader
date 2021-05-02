Nitro Bootloader
================

This is a bootloader meant to allow DFU flashing of both bitstream and
other data (such as softcore firmware for instances) on FPGA boards where
a couple of the FPGA pins are used to implement USB through a softcore.


Limitations
-----------

The current code targets the iCE40 FPGA boards only.

A modified version was already used on the ECP5 for the Hack-a-day
badge 2019 supercon but the improvements still need to be integrated
here.

If you're interested in porting to other platforms, don't hesistate
to contact me.


Instructions
------------

If you are looking for the DFU bootloader for iCE40 based boards,
refer to the build process described in the
[ice40-stub README](gateware/ice40-stub/README.md).


License
-------

See LICENSE.md for the licenses of the various components in this repository

Components imported by sub-modules might be subject to their own license.
Check the various `cores` in `gateware/` directory.

Note that theses obviously only apply to the bootloader gw/fw itself and
NOT to the hardware or to whatever bitstream/firmware it loads.
Licenses for those can be anything the user wants.
