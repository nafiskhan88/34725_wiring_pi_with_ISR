
#include        "Adafruit_TCS34725.cpp"
#include        <stdio.h>
#include        <stdlib.h>
#include        <inttypes.h>
#include        <stdint.h>
#include        <time.h>
#include        <unistd.h>
#include        <math.h>
#include        <wiringPi.h>
#include        <wiringPiI2C.h>






int main ()

{


      //Setting GPIO for INT

        //pinMode (0, INPUT) ;

        #if 0
        uint8_t device_ID;
        
        uint16_t clear_data;

        uint16_t red_data;

        uint16_t green_data;

        uint16_t blue_data;

        #endif

        FILE* logfile;

        logfile =fopen("test.txt", "w");

      
       
        uint16_t r, g, b, c, colorTemp, lux;
      
        Adafruit_TCS34725_enable();

        
        init_sensor();

        
        printf("Device ID %x\n", read8 (TCS34725_ID)); 

        printf(" Register 0x00 before calling Interrupt  :%x\n", read8(TCS34725_ENABLE) );







        interrupt_limit ( 0xFFFF, 0x0000);  // low is decimal 300 and high is decimal 200. It should trigger interrupt if the value is betn 200 and 300.

        printf("Interrupt Threshold %x %x, %x %x \n", read8(0x04), read8(0x05) , read8(0x06), read8(0x07) );

        clear_interrupt();

        printf("Status Register 0x13 after clear interrupt %x\n", read8(TCS34725_STATUS) );

        init_interrupt ();

        printf(" Register 0x00 after calling Interrupt  :%x\n", read8(TCS34725_ENABLE) );

        printf("Status Register 0x13 before clear interrupt %x\n", read8(TCS34725_STATUS) );

        

        

        
        

        
        //getRawData(&r, &g, &b, &c);
        //colorTemp = calculateColorTemperature(r, g, b);
        //lux = calculateLux(r, g, b);


       for (;;) 

        {   
                      #if 1
                      delay (100);

                      uint8_t status = read8 (TCS34725_STATUS);

                      getRawData(&r, &g, &b, &c);

                      colorTemp = calculateColorTemperature(r, g, b);

                      lux = calculateLux(r, g, b);
 
                      printf(" Staus %x Clear %d R %d  G %d  B %d lux %d  colorTemp %d\n", status,  c , r, g, b, lux, colorTemp );

                      fprintf(logfile, "Clear data = %d | Red_data = %d | Green_data = %d | Blue_data = %d | lux = %d | colorTemp = %d \n", c , r, g , b , lux , colorTemp);
                      
                      

                      //if (status = 0x11)
                            clear_interrupt();
                          //init_sensor();



                      #endif

                        #if 0
                    
                          clear_data = read16(TCS34725_CDATAL);

                          red_data = read16(TCS34725_RDATAL);

                          green_data = read16(TCS34725_GDATAL);

                          blue_data = read16(TCS34725_BDATAL);

                    
                        #elseif
                        clear_data = wiringPiI2CReadReg16(fd, TCS34725_CDATAL);

                        red_data = wiringPiI2CReadReg16(fd, TCS34725_RDATAL);

                        green_data = wiringPiI2CReadReg16(fd, TCS34725_GDATAL);

                        blue_data = wiringPiI2CReadReg16(fd, TCS34725_BDATAL);

                        #endif

                        //printf("Clear data = %x Red_data = %x Green_data = %x Blue_data = %x \n", clear_data , red_data, green_data , blue_data );
                        
                        //fprintf(logfile, "Clear data = %x | Red_data = %x | Green_data = %x | Blue_data = %x |\n", clear_data , red_data, green_data , blue_data);




                          //printf(" Red_data = %x\n" , red_data );

                          //printf(" Green_data = %x\n" , green_data);

                          //printf(" Blue_data = %x\n ", blue_data);

                     
                       } 
                
          


        
        return 0;
}