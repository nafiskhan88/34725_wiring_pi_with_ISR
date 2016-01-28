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

	uint16_t r, g, b, c;




	for (;;)
                {
                    
                	getRawData (&r , &g, &b, &c);

                    printf("Clear data = %x\n", c);

                    printf(" Red_data = %x\n" , r);

                    printf(" Green_data = %x\n" , g);

                    printf(" Blue_data = %x\n ", b);   

                    delay (500); 
                        
                }




	return 0;
}


