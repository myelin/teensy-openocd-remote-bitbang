# teensy-openocd-remote-bitbang

Copyright (C) 2016 Phillip Pearson <pp@myelin.co.nz>

The teensy-openocd-remote-bitbang.ino sketch implements the OpenOCD JTAG remote_bitbang protocol using a USB serial port, so you can use a Teensy as a simple USB JTAG adapter with OpenOCD and socat.

Run it on a Teensy 3.1 or 3.2 from PJRC: https://www.pjrc.com/teensy/teensy31.html

# For more debugging info

python remote_bitbang_serial_debug.py -P 3335 /dev/tty.usbmodem*
