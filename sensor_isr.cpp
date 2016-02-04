
/*
 * isr.c:
 *  Wait for Interrupt test program - ISR method
 *
 *  How to test:
 *    Use the SoC's pull-up and pull down resistors that are avalable
 *  on input pins. So compile & run this program (via sudo), then
 *  in another terminal:
 *    gpio mode 0 up
 *    gpio mode 0 down
 *  at which point it should trigger an interrupt. Toggle the pin
 *  up/down to generate more interrupts to test.
 *
 * Copyright (c) 2013 Gordon Henderson.
 ***********************************************************************
 * This file is part of wiringPi:
 *  https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */


#include          <string.h>
#include          <errno.h>
#include          "Adafruit_TCS34725.cpp"
#include          <stdio.h>
#include          <stdlib.h>
#include          <inttypes.h>
#include          <stdint.h>
#include          <time.h>
#include          <wiringPi.h>
#include          <wiringPiI2C.h>
#include          "Adafruit_TCS34725.h"
#include          <math.h>


// What GPIO input are we using?
//  This is a wiringPi pin number

#define BUTTON_PIN  0

// globalCounter:
//  Global variable to count interrupts
//  Should be declared volatile to make sure the compiler doesn't cache it.

static volatile int globalCounter = 0 ;


/*
 * myInterrupt:
 *********************************************************************************
 */

void myInterrupt (void)
{
  //printf("Global_counter from Interrupt function = %d\n",globalCounter );
  uint16_t r, g, b, c, colortemp, lux;
  getRawData (&r , &g, &b, &c);
  colortemp = calculateColorTemperature (r , g, b);
  lux = calculateLux (r , g, b);

                    printf("Clear data hex = %x , decimal = %d : Red_data = %x : Green_data = %x : Blue_data = %x : \n", c , c, r , g, b);
                    printf(" Color Temp %d : Lux %d \n",colortemp, lux );

          
  globalCounter++ ;

}


/*
 *********************************************************************************
 * main
 *********************************************************************************
 */

int main (void)
{


  time_t rawtime;

  struct tm * timeinfo;
  
  //enable sensor

  Adafruit_TCS34725_enable();  //will power up first , then wait 3ms then enable RGBC and gain

  interrupt_limit (100, 1000); //first low then high value

  init_interrupt (); //PERS_3_CYCLE

  

  

  int myCounter = 0 ;

  #if 1

  if (wiringPiSetup () < 0)
  {
    fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }

  #endif

  if (wiringPiISR (BUTTON_PIN, INT_EDGE_FALLING, &myInterrupt) < 0)
  {
    fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ;
    return 1 ;
  }


  for (;;)
  {
    printf ("Waiting ... \n") ; fflush (stdout) ;

    //printf("before condition match :mycounter value %d Global_counter %d \n", myCounter, globalCounter );

    while (myCounter == globalCounter)
      delay (10) ;

    printf (" Done. counter: %5d\n", globalCounter) ;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf ( "Interrupt happened on %s \n", asctime (timeinfo) );
    myCounter = globalCounter ;
    clear_interrupt(); 
  }

  return 0 ;
}
