#include        <stdio.h>
#include        <stdlib.h>
#include        <inttypes.h>
#include        <stdint.h>
#include	      <time.h>
#include 	      <wiringPi.h>
#include 	      <wiringPiI2C.h>
#include 	      "Adafruit_TCS34725.h"




int fd;


void Adafruit_TCS34725_enable(void)
{
  

  wiringPiSetup () ;
        

  fd = wiringPiI2CSetup (TCS34725_ADDRESS) ;


  wiringPiI2CWriteReg8(fd, (0x80|0x00), (0xFF & 0x01));
  printf(" Enabled register  %x\n", wiringPiI2CReadReg8(fd, 0x00));

  delay(3);

  wiringPiI2CWriteReg8 (fd, (0x80 |0x00), ( 0x02)); 
  printf(" Enabled register after RGBC Enabled %x\n", wiringPiI2CReadReg8(fd, 0x00)); 

  


}






void init_sensor (void)

{

//Enable wait timer form register 0x00

  wiringPiI2CWriteReg8 (fd, TCS34725_ENABLE, 0x80|TCS34725_ENABLE_WEN); //bit 3 is set on register 0x00

//set gain
  wiringPiI2CWriteReg8 (fd, TCS34725_CONTROL , 0x80|0x00);  //set it to 1X gain

//set integration time on register 0x01
  wiringPiI2CWriteReg8 (fd, TCS34725_ATIME, 0x80|0xFF);

//enable wait time register
  wiringPiI2CWriteReg8 (fd, (0x80|TCS34725_WTIME), (0xFF & TCS34725_WTIME_2_4MS));

  uint8_t x = wiringPiI2CReadReg8 (fd, 0x80|TCS34725_ID);
  if (x != 0x44)
  {
    printf ("Device ID Mismatch");
  }

  printf ("Init done !!! ");
}






int main ()
{


      
     

	     
        uint16_t clear_data;

        uint16_t red_data;

        uint16_t green_data;

        uint16_t blue_data;
       
        

       
        Adafruit_TCS34725_enable();
        printf("%d\n",fd );
       

        //wiringPiI2CWriteReg8(fd, 0x00, (0x80|0x01));
        //delay(300);

        //wiringPiI2CWriteReg8 (fd, 0x00, (0x80 | 0x02));

        


        printf(" Device ID Register %x \n", wiringPiI2CReadReg8(fd, (0x80|TCS34725_ID )));
        printf(" Enabled register after RGBC Enabled %x \n", wiringPiI2CReadReg8(fd, (0x80|0x00)));
        //init_sensor();

        

      for (;;)
                {
                        clear_data = wiringPiI2CReadReg16(fd, TCS34725_CDATAL);

                        red_data = wiringPiI2CReadReg16(fd, TCS34725_RDATAL);

                        green_data = wiringPiI2CReadReg16(fd, TCS34725_GDATAL);

                        blue_data = wiringPiI2CReadReg16(fd, TCS34725_BDATAL);


                         printf("Clear data = %x\n", clear_data);

                         printf(" Red_data = %x\n" , red_data );

                        printf(" Green_data = %x\n" , green_data);

                        printf(" Blue_data = %x\n ", blue_data);
                        
                }


        
        //fd=wiringPiI2CSetup (TCS34725_ADDRESS) ;  /*Use i2cdetect command to find your respective device address*/

        /*
        if(fd==-1)

        {
                printf("Can't setup the I2C device\n");

                return -1;
        }
        else
        {
                for (;;)
                {
                        clear_data = wiringPiI2CReadReg16(1, TCS34725_CDATAL);

                        red_data = wiringPiI2CReadReg16(1, TCS34725_RDATAL);

                        green_data = wiringPiI2CReadReg16(1, TCS34725_GDATAL);

                        blue_data = wiringPiI2CReadReg16(1, TCS34725_BDATAL);
                        
                        
                        if(clear_data == -1)

                        {
                                printf("No data\n");

                                //return -1;
                        }

                        else
                        {
                                //print data
                                printf("Clear data = %d\n", clear_data);

                                printf(" Red_data = %d\n" , red_data );

                                printf(" Green_data = %d\n" , green_data);

                                printf(" Blue_data = %d\n ", blue_data);
                        }
                }       
        }

        
      */

        return 0;
}
