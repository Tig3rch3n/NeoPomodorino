# NeoPomodorino
90% of the Work goes to https://github.com/atbrask/ with his nice Implimentation.

A Friend asked me if I could build him a [Pomodoro style timer](https://en.wikipedia.org/wiki/Pomodoro_Technique)

BOM:
~10cm DN75 'HT-Rohr' The German Standard Drainpipe (cut it in a 55° Angle)
3D Printed Top/Bottom
Lasercut/CNC Milled Acrylic Front
Adafruit Neopixel with 24 LEDs
7-10 Dupont-Jumper Wire
Cherry MX Switch
USB-Micro Male Breakout
USB-C Female Breakout
Arduino Nano
Piezo Buzzer

I developed my own Housing and plan to add a Piezo Buzzer that does a short 'Blip' noise as the Work & Break timer runs out. (But not Working yet)
Each 30/10/5s before the Work/Break switch.
No PCB Required, I just cut some Dupont-Jumper wire and Soldered them directly to the Neopixel Ring/Switch/Buzzer.

The Tiny USB-Micro to USB-C is a bit Tricky, use some Flux-Paste to make your work easyer.
![Tiny Adapter](Images/Adapter_Cable.jpg)
Cut a Slot in the Back of your Housing and Hotglue/2k Epoxy the USB-Adapter and the Arduino to the Housing Bottom as well as the Buzzer somewere to the Back of the Frontplate.
If the Switch is to loose in the Faceplate, glue it as well, but don't use Superglue the Vapor will damage the Switch and it will be unreliable.

The Connections:
A Neopixel has three Connections, VCC is 5V, GND goes to GND and Sig/IN goes to Pin 6 of the Arduino.
The Switch goes to GND and Pin 2 and the Buzzer goes to GND and to Pin 3 with a 100Ohm Resistor in Line.

The Prints take about 1 1/2h and ~29g PETG.

![First Render](Images/Early_Render.png)
![The Inside](Images/Inside.jpg)



### Usage

* Long button presses (> 500ms) will turn the device on and off. This also resets the timer.
* Short button presses will add 1 minute to the current timer.
* Each pomodoro consists of 25 minutes of work (cyan-green'ish color) followed by a short 5 minute pause (orange color).
* Every fourth pause is a longer one of 25 minutes (white color).
* Between each pomodoro, the device will briefly flash which number is (blue color). 1 quarter of the ring = 1 pomodoro.
