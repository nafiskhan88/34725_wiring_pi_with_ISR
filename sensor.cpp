#include 		"Adafruit_TCS34725.cpp"
#include        <stdio.h>
#include        <stdlib.h>
#include        <inttypes.h>
#include        <stdint.h>
#include	    <time.h>
#include 	    <wiringPi.h>
#include 	    <wiringPiI2C.h>
#include 	    "Adafruit_TCS34725.h"




int main ()

{

	//enable sensor

	Adafruit_TCS34725_enable();  //will power up first , then wait 3ms then enable RGBC and gain

	interrupt_limit (100, 2000); //first low then high value

	init_interrupt (); //PERS_3_CYCLE


	return 0;
}


