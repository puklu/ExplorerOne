## Unable to flash (STLINK_SWD_DP_ERROR)
If for some reason you are unable to flash because of some stlink issue caused
by some misconfiguration of some GPIO pin becasue of the firmware, while holding down 
the reset button down, run
`st-flash --connect-under-reset erase`
You might see something like, this means it was successful
```
Mass erasing...
Mass erase completed successfully.
Failed to reset device
```
