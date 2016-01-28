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
	uint16_t status;



	for (;;)
                {
                    
                	status = read8(TCS34725_STATUS);

                	printf (" Status Register  %x \n", read8(TCS34725_STATUS)) ;

                	if ((status & 0x10) == 0x10)

                	{
                	getRawData (&r , &g, &b, &c);

                    printf("Clear data :%x\n Red data :%x\n Green data :%x\n Blue data :%x\n", c, r, g, b);

                    clear_interrupt(); 

                    delay (500);

                	}




                    #if 0
                	getRawData (&r , &g, &b, &c);

                    printf("Clear data = %x\n", c);

                    printf(" Red_data = %x\n" , r);

                    printf(" Green_data = %x\n" , g);

                    printf(" Blue_data = %x\n ", b);   

                    delay (500); 

                    #endif
                        
                }




	return 0;
}


