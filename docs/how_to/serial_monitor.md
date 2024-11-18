# To start an terminal emulator to capture data coming from serial

All of this can be done in the container as well since dev ports ACM* and mapped to
the container as well.

## Simplest way is to run it as a vscode task

```
CTRL+Shift+B and select serial-monitor
```

If it doesn't work some reason, like the port may not be ttyACM1 as is defined 
in the vscode task, then the following steps may be followed.

## Check the port number
``` 
ls /dev/ttyACM*
```

If unsure, unplug your device, run ls /dev/tty*, and plug it back in to see which device appears.

## Open the emulator
If the serial to USB device is, say connected to ttyACM1, then

```
minicom -D /dev/ttyACM1 -b 115200
```

where 11520 is the baud rate.


### Tips
- You might want to enable carriage return

```
CTRL-A Z

followed by

U
```

- To exit the emulator

```
CTRL-A Z

followed by

X
```