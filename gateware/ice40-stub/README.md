Requirements
------------

To build the ice40 DFU bootloader you will need the YosysHQ toolchain and an
embedded risc-v cross compiler with newlib.

You can download pre built binaries from
[YosysHQ fpga-toolchain](https://github.com/YosysHQ/fpga-toolchain/releases).

And you can download the risc-v embedded toolchain from
[xPack](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases).


Build
-----

To build the DFU bootloader run:
```
make bootloader
```

By default this will build the DFU bootloader for the full size iCEBreaker FPGA
development board. If you want to build for a different platform you will have
to provide the `BOARD=` parameter.

For example to build the DFU bootloader for the iCEBreaker-bitsy V1.x you can run:
```
make BOARD=bitsy-v1 bootloader
```

In some cases the risc-v cross compiler has a different prefix than the default
`riscv-none-elf-`. You can adjust the prefix to the one you have installed on
your system using the `CROSS=` parameter.

For example on Arch Linux the riscv crosscompiler has the prefix
`riscv64-unknown-elf-` so you would build the bootloader by running:
```
make CROSS=riscv64-unknown-elf- bootloader
```


Clean
-----

In some cases make does not recognize that it needs to rebuild the bitstreams
and firmware. For example when you change the `BOARD=` parameter. To clean all
the generated binary artifacts and force a full rebuild of the bootlader you
can run the following command:

```
make PRE_CLEAN=1 bootloader-clean
```

Flash
-----

To flash the bootloader onto a target using `iceprog` you can run the following
command:
```
make prog-bootloader
```
or, if you need to run `iceprog` with root permissions:
```
make sudo-prog-bootloader
```

TODO: Add instructions on how to update the bootloader using `dfu-util`.
