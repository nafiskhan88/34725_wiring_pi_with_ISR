# 34725_wiring_pi_with_ISR
Adafruit RGB sensor (34725) code for Raspberry pi using wiiring Pi library

This code can will work with Adafruit RGB sensor 34725 and is written in C using Wiring Pi library and tested with Raspberry Pi 2.
Interrupt from the chip is connected to GPIO 0. Interrupt is Active low .

if the interrupt value (clear channel data) is below a 'low' threshold and above 'High' threshhold then it will generate an interrupt. 
Interrupt is triggered based on clear channel value.

TODO:
detect color and trigger interrupt based on that.

