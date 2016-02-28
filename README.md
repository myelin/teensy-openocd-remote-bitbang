# teensy-openocd-remote-bitbang

Copyright (C) 2016 Phillip Pearson <pp@myelin.co.nz>

This sketch implements the OpenOCD JTAG remote_bitbang protocol using a USB serial port, so you can use a Teensy as a simple USB JTAG adapter with OpenOCD and socat.

Run it on a Teensy 3.1 or 3.2 from PJRC: https://www.pjrc.com/teensy/teensy31.html

# Also included

From PySerial: tcp_serial_redirect.py (C) 2002-2015 Chris Liechti <cliechti@gmx.net>, BSD-3-Clause license.

python tcp_serial_redirect.py -P 3335 /dev/tty.usbmodem*
