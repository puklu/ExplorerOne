# Install the ARM Toolchain
```
sudo apt update
sudo apt install gcc-arm-none-eabi gdb-multiarch
```

# Install OpenOCD
OpenOCD (Open On-Chip Debugger) is used for programming and debugging STM32 microcontrollers.
```
sudo apt install openocd

```

# Start OpenOCD
To start OpenOCD, use the following command, specifying the interface and target configuration files:
```
openocd -f interface/stlink.cfg -f board/stm32f3discovery.cfg
```

This terminal would be occupied after this.

# Use GDB to Flash the Device

1) In another terminal, run GDB with your binary:
```
gdb-multiarch build/src/ExplorerOne.elf
```

2) In the GDB session, connect to the OpenOCD server:
```
target remote localhost:3333
```

3) Flash the firmware:
```
load
```

4) Run the program:
```
monitor reset init
continue

```

or

```
break main # stops at entrypoint, which is main
continue
```

# OR Use telnet to Flash the Device

1) With OpenOCD started in another teminal, connect via telnet:
```
telnet localhost 4444

```

2) Program the device (e.g., flashing firmware.bin):
```
flash write_image erase /path/to/firmware.bin 0x08000000

```
Here, 0x08000000 is the base address of the flash. Can check in the linker script

3) Verify the flash:
```
verify_image /path/to/firmware.bin 0x08000000

```

4) Reset and run the program:
```
reset run
```

Note: OpenOCD should be opened in the build directory where the binary is.