/*
 * This sketch implements the OpenOCD remote_bitbang protocol using a USB serial port.
 * Run it on a Teensy 3.1 or 3.2 from PJRC: https://www.pjrc.com/teensy/teensy31.html
 * 
 * Copyright (C) 2016 Phillip Pearson <pp@myelin.co.nz>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * I have this connected up using my swd-to-wires breakout board:
 *   https://github.com/myelin/breakout-boards/tree/master/swd-to-wires
 * On the swd-to-wires board: grey=gnd, purple=rst, orange=swdio/tms, yellow=tck, green=tdo, blue=tdi
 * Hooked those in side by side on a Teensy 3.2 so grey=gnd, purple=0=srst, blue=1=tdi, green=2=tdo, yellow=3=tck, orange=4=tms
 * 
 * To run:
 * - Download the sketch to your Teensy
 * - Test with minicom -D /dev/tty.usbmodem1485121 (replace with your device name).  The LED should come on if you hit B, and go off if you hit b.
 * - In one terminal: socat -d -d -d file:/dev/tty.usbmodem1485121,clocal=1,cs8,nonblock=1,ixoff=0,ixon=0,ispeed=9600,ospeed=9600,raw,echo=0,crtscts=0 tcp-listen:3335,reuseaddr
 *   Or for more debug info: sudo pip install pyserial; python remote_bitbang_serial_debug.py -P 3335 /dev/tty.usbmodem*
 * - In another: openocd -d -c "reset_config srst_only; interface remote_bitbang; remote_bitbang_port 3335; remote_bitbang_host localhost" [-f ...]
 * 
 * ALTERNATIVELY YOU CAN USE THIS AS A USB-SERIAL BRIDGE TO PROGRAM YOUR ESP8266!
 *
 * Uncomment the SERIAL_BRIDGE define below and connect your Teensy to your ESP8266 as follows:
 *
 * - gray = teensy pin 5 --> esp8266 CH_PD
 * - purple = teensy pin 6 --> esp8266 GPIO0
 * - blue = RX3 = teensy pin 7 --> esp8266 TX
 * - green = TX3 = teensy pin 8 --> esp8266 RX
 */

#define JTAG_BITBANG
//#define SERIAL_BRIDGE

#ifdef JTAG_BITBANG
#ifdef SERIAL_BRIDGE
#error JTAG_BITBANG and SERIAL_BRIDGE functions don't work together -- please choose just one.
#endif // SERIAL_BRIDGE
#define PIN_SRST 0
#define PIN_TDI 1
#define PIN_TDO 2
#define PIN_TCK 3
#define PIN_TMS 4
#endif //JTAG_BITBANG

#define PIN_LED 13

#ifdef SERIAL_BRIDGE
#define HWSerial Serial3

// DTR connects to GPIO0 on the ESP8266
#define PIN_GPIO0 6

// RTS connects to CH_PD/CHIP_EN on the ESP8266
#define PIN_CHIP_EN 5

uint32_t current_baud = 9600;
uint8_t current_dtr = -1, current_rts = -1;
#endif // SERIAL_BRIDGE

void setup() {
  // Init the Teensy USB serial port
  Serial.begin(9600); // fake value -- USB always 12Mbit

#ifdef JTAG_BITBANG
  // Set up the JTAG pins
  pinMode(PIN_SRST, OUTPUT);
  pinMode(PIN_TDI, OUTPUT);
  pinMode(PIN_TDO, INPUT_PULLUP);
  pinMode(PIN_TCK, OUTPUT);
  pinMode(PIN_TMS, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_SRST, HIGH);
  digitalWrite(PIN_TDI, HIGH);
  digitalWrite(PIN_TCK, LOW);
  digitalWrite(PIN_TMS, HIGH);
  digitalWrite(PIN_LED, HIGH);
#endif // JTAG_BITBANG

#ifdef SERIAL_BRIDGE
  HWSerial.begin(9600);
  pinMode(PIN_GPIO0, OUTPUT);
  digitalWrite(PIN_GPIO0, HIGH);
  pinMode(PIN_CHIP_EN, OUTPUT);
  digitalWrite(PIN_CHIP_EN, HIGH);
#endif // SERIAL_BRIDGE

}

void loop() {
#ifdef JTAG_BITBANG
  while (Serial.available() > 0) {
    uint8_t c = Serial.read();
    switch (c) {
    case 'B':
      digitalWrite(PIN_LED, HIGH);
      break;
    case 'b':
      digitalWrite(PIN_LED, LOW);
      break;
    case 'R':
      Serial.write((digitalRead(PIN_TDO) == HIGH) ? '1' : '0');
      break;
    case 'Q':
      break;
    case '0':
      digitalWrite(PIN_TMS, LOW);
      digitalWrite(PIN_TDI, LOW);
      digitalWrite(PIN_LED, LOW);
      digitalWrite(PIN_TCK, LOW);
      break;
    case '1':
      digitalWrite(PIN_TMS, LOW);
      digitalWrite(PIN_TDI, HIGH);
      digitalWrite(PIN_LED, LOW);
      digitalWrite(PIN_TCK, LOW);
      break;
    case '2':
      digitalWrite(PIN_TMS, HIGH);
      digitalWrite(PIN_TDI, LOW);
      digitalWrite(PIN_LED, LOW);
      digitalWrite(PIN_TCK, LOW);
      break;
    case '3':
      digitalWrite(PIN_TMS, HIGH);
      digitalWrite(PIN_TDI, HIGH);
      digitalWrite(PIN_LED, LOW);
      digitalWrite(PIN_TCK, LOW);
      break;
    case '4':
      digitalWrite(PIN_TMS, LOW);
      digitalWrite(PIN_TDI, LOW);
      digitalWrite(PIN_LED, HIGH);
      digitalWrite(PIN_TCK, HIGH);
      break;
    case '5':
      digitalWrite(PIN_TMS, LOW);
      digitalWrite(PIN_TDI, HIGH);
      digitalWrite(PIN_LED, HIGH);
      digitalWrite(PIN_TCK, HIGH);
      break;
    case '6':
      digitalWrite(PIN_TMS, HIGH);
      digitalWrite(PIN_TDI, LOW);
      digitalWrite(PIN_LED, HIGH);
      digitalWrite(PIN_TCK, HIGH);
      break;
    case '7':
      digitalWrite(PIN_TMS, HIGH);
      digitalWrite(PIN_TDI, HIGH);
      digitalWrite(PIN_LED, HIGH);
      digitalWrite(PIN_TCK, HIGH);
      break;
    case 'r':
    case 't':
      // SRST=0, which confusingly means to *exit* reset (as /RESET and /TRST are active-low)
      // We don't have a TRST connection, so 'r' and 't' do the same thing.
      digitalWrite(PIN_SRST, HIGH);
      //digitalWrite(PIN_CHIP_EN, HIGH);
      break;
    case 's':
    case 'u':
      // SRST=1 -- enter RESET state
      // Likewise for 's' and 'u'.
      digitalWrite(PIN_SRST, LOW);
      //digitalWrite(PIN_CHIP_EN, LOW);
      break;
      /*
    case 'X':
      // esp8266 reset
      pinMode(PIN_TDO, INPUT_PULLUP);
      digitalWrite(PIN_GPIO0, HIGH);
      digitalWrite(PIN_SRST, LOW);
      digitalWrite(PIN_CHIP_EN, LOW);
      delay(100);
      digitalWrite(PIN_SRST, HIGH);
      digitalWrite(PIN_CHIP_EN, HIGH);
      break;
      */
    }
  }
#endif // JTAG_BITBANG

#ifdef SERIAL_BRIDGE
  if (Serial.available() > 0) {
    uint8_t c = Serial.read();
    HWSerial.write(c);
  }
  while (HWSerial.available() > 0) {
    Serial.write(HWSerial.read());
  }
  // update params
  uint32_t baud = Serial.baud();
  if (baud != current_baud) {
    current_baud = baud;
    HWSerial.begin(current_baud);
  }
  uint8_t rts = Serial.rts();
  if (rts != current_rts) {
    current_rts = rts;
    digitalWrite(PIN_CHIP_EN, current_rts ? LOW : HIGH);
  }
  uint8_t dtr = Serial.dtr();
  if (dtr != current_dtr) {
    current_dtr = dtr;
    digitalWrite(PIN_GPIO0, current_dtr ? LOW : HIGH);
  }
#endif // SERIAL_BRIDGE
}

