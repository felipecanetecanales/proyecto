#include "msp430_version.h"            // Depende del uC que Ud. esté ocupando.

#include "i2c.h"
#include "osc.h"
#include <stdio.h>
#include "display.h"
#include  <msp430x16x.h>

char buffer[10];

void main (void)
{
  osc_init_xt2();
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//  display_init();
//  display_set_pos(0x00);
//  TACCTL0 = CCIE;
  TACTL = TASSEL_1 + MC_1;    
  TACCR0 = 32768-1;
  i2c_init(0x68,I2C_MODE_MASTER);
//  _BIS_SR(GIE);

  buffer[0] = 0x00;
  buffer[1] = 0x24;
  buffer[2] = 0x12;
  buffer[3] = 0x02;
  buffer[4] = 0x12;
  buffer[5] = 0x12;
  buffer[6] = 0x17; 
//  i2c_master_write(0x00,buffer,7); 
  while(i2c_is_busy());
  i2c_master_read(0x00,buffer,7);
  while(1);
}


#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
//  while(i2c_is_busy());
  i2c_master_read(0x00,buffer,7);
  display_clear();
  printf("%02x",buffer[0]);
  
}