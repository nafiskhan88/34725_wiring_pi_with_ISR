#include        <stdio.h>
#include        <stdlib.h>
#include        <inttypes.h>
#include        <stdint.h>
#include        <time.h>
#include        <unistd.h>
#include        <math.h>
#include        <wiringPi.h>
#include        <wiringPiI2C.h>
#include        "Adafruit_TCS34725.h"





int fd;


/* 8 bit Register Write   */

void write8(uint8_t reg, uint16_t value)

{

  wiringPiI2CWriteReg8(fd,(TCS34725_COMMAND_BIT|reg), (value & 0xFF));

  //return 0;

  //int wiringPiI2CWriteReg8 (int fd, int reg, int data) ;


}


/* 8 bit Register Read   */
uint8_t read8(uint8_t reg)
{

  uint8_t value8;


  value8 = wiringPiI2CReadReg8 (fd , (TCS34725_COMMAND_BIT|reg));

  // int wiringPiI2CReadReg8 (int fd, int reg) ; for reference

  return value8;

}




/* 16 bit Register Read   */
uint16_t read16(uint8_t reg)
{
  uint16_t value16;

  value16 = wiringPiI2CReadReg16 ( fd , (TCS34725_COMMAND_BIT | reg ) );

  return value16;

}



/**************************************************************************/
/*!
    Enables the device, added wiringpi setup into this.
    fd must have been initialized.
*/
/**************************************************************************/
void Adafruit_TCS34725_enable(void)

{

  wiringPiSetup ();                         //setup I2C for Raspberry pi. This needs to be called at the very beginning
        

  fd = wiringPiI2CSetup (TCS34725_ADDRESS) ; 

  printf("%d\n", fd);

  #if 1

  uint8_t x = read8 (TCS34725_ID);
  
  if (x != 0x44)
  {
    printf ("Device ID Mismatch");

    else

      printf("Device ID = %d\n", x);
  }


  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  //wiringPiI2CWriteReg8(fd, (0x80|0x00), (0xFF & 0x01));

  printf(" Register 0x00 After Enable, Bit 0: %x\n", read8((TCS34725_ENABLE )));   //sanity checking

  delay (3);

  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);  

  //wiringPiI2CWriteReg8 (fd, (0x80 |0x00), ( 0x02));

  printf(" Register 0x00  after RGBC enabled, Bit 1: %x\n", read8((TCS34725_ENABLE )));

  #endif
}





void Adafruit_TCS34725_disable(void)
{
  /* Turn the device off to save power */

  uint8_t reg = 0;

  reg = read8(TCS34725_ENABLE);

  write8(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}



/* Set gain and integration time   */

void init_sensor (void)

{

//Enable wait timer form register 0x00

  write8 (TCS34725_ENABLE, (TCS34725_ENABLE_WEN | TCS34725_ENABLE_AEN | TCS34725_ENABLE_PON));     //bit 3 is set on register 0x00

  printf(" Register 0x00 After writing WEN , Bit 3 : %x\n", read8((TCS34725_ENABLE )));

//set gain
  write8 (TCS34725_CONTROL , 0x00);  //set it to 1X gain

  printf(" Control Register Gain Value : %x\n", read8((TCS34725_CONTROL)));

//set integration time on register 0x01
  write8 (TCS34725_ATIME, 0xD5);  //101ms

  printf(" Register (0x01) Integration time (0xFF = 2.4ms) : %x\n", read8((TCS34725_ATIME)));

  //enable wait time register
  write8(TCS34725_WTIME, TCS34725_WTIME_2_4MS);

  printf(" Register WTIME (0xFF = 2.4ms) : %x\n", read8((TCS34725_WTIME )));

  //Write to config register, make the WLONG = 0
  write8(TCS34725_CONFIG , 0x00);

  printf(" Config Register (0x0D) : %x\n", read8((TCS34725_WTIME )));

  
}




float powf(const float x, const float y)
{
  return (float)(pow((double)x, (double)y));
}







uint16_t calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b)
{
  float X, Y, Z;      /* RGB to XYZ correlation      */
  float xc, yc;       /* Chromaticity co-ordinates   */
  float n;            /* McCamy's formula            */
  float cct;

  /* 1. Map RGB values to their XYZ counterparts.    */
  /* Based on 6500K fluorescent, 3000K fluorescent   */
  /* and 60W incandescent values for a wide range.   */
  /* Note: Y = Illuminance or lux                    */
  X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
  Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
  Z = (-0.68202F * r) + (0.77073F * g) + ( 0.56332F * b);

  /* 2. Calculate the chromaticity co-ordinates      */
  xc = (X) / (X + Y + Z);
  yc = (Y) / (X + Y + Z);

  /* 3. Use McCamy's formula to determine the CCT    */
  n = (xc - 0.3320F) / (0.1858F - yc);

  /* Calculate the final CCT */
  cct = (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

  /* Return the results in degrees Kelvin */
  return (uint16_t)cct;
}

/**************************************************************************/
/*!
    @brief  Converts the raw R/G/B values to color temperature in degrees
            Kelvin
*/
/**************************************************************************/
uint16_t calculateLux(uint16_t r, uint16_t g, uint16_t b)
{
  float illuminance;

  /* This only uses RGB ... how can we integrate clear or calculate lux */
  /* based exclusively on clear since this might be more reliable?      */
  illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

  return (uint16_t)illuminance;
}


void getRawData (uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
  //if (!_tcs34725Initialised) begin();

  *c = read16(TCS34725_CDATAL);

  *r = read16(TCS34725_RDATAL);

  *g = read16(TCS34725_GDATAL);

  *b = read16(TCS34725_BDATAL);

  delay (3);      //wait 3 ms for the Integration time
  
  
}





void init_interrupt (void)

{

write8 ( TCS34725_ENABLE, (TCS34725_ENABLE_AIEN| TCS34725_ENABLE_WEN |  TCS34725_ENABLE_AEN | TCS34725_ENABLE_PON)); //bit 4, 3, 1,0 is set. value should be 0x1B

printf (" Register 0x00 after Interrupt Enable:  %x \n" , read8(TCS34725_ENABLE)) ;


write8 (TCS34725_PERS , 0x05);  //0b0101, 10 clear channel values outside threshhold values

printf (" Register 0x0C after Setting counter:  %x \n", read8(TCS34725_PERS)) ;

}




void interrupt_limit (uint16_t low, uint16_t high)

{

    write8(0x04, low & 0xFF);

    write8(0x05, (low & 0xFF00 )>> 8);

    write8(0x06, high & 0xFF);

    write8(0x07, (high & 0xFF00) >> 8);


}


void clear_interrupt()
{

write8 ( TCS34725_ADDRESS, 0xE6);  //Use command register to clear interrupt. bit 5:6 is 11,and 0:4 is 00110


}
