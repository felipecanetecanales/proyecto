#include "msp430_version.h" 
#include "osc.h"
#include "adc.h"
#include "display.h"
#include "i2c.h"
#include <stdio.h>
#include "keyboard.h"
#include "tic.h"
#include "RTC.h"


int compuerta_ctl=0; 
int a,b,d;
int tacr_nuevo, tacr_viejo;
int cuerpo;
int tiempo_alcanzado=0;
int ya_iniciado = 0;
//int contador_distancia_gato = 0;
float res;
int cat_detected;
long unsigned int contador_main=0;
unsigned int contador_reloj=0;
char buffer[10];
extern char hora1_bcd,hora2_bcd,hora3_bcd,min1_bcd,min2_bcd,min3_bcd, modo;
extern int gato_detectado;
/*
void compuerta_servo(int control){
  
  if(control==1){    
    TACCR1= 32;
    }
    else if(control==0){
    TACCR1=65;
    }
}
*/
void main(void){
    
    osc_init_xt2();
    keyboard_init();
    //tic_init();
    WDTCTL = WDTPW + WDTHOLD;
    
    P5DIR |= 0x78;
    P4DIR |= 0xF8;
    P2DIR |= 0x1F;
    P2SEL |= BIT3;
    P2OUT &= ~0x17;
    P5OUT &= ~BIT4;
    P5OUT |= BIT3;
    P5OUT &= ~BIT5;
    display_init();
    
    
    i2c_init_it(0x68,I2C_MODE_MASTER);

 
    TACCTL0 = CCIE;
    TBCCTL0 = CCIE;
    
    TACCR0 = 655-1;
    TACTL = TASSEL_1 + MC_1 + TAIE;    // reloj ACLK, modo up to CCR0
    TACCTL1 = OUTMOD_7;         //PWM para servo motor de compuerta
    TACCR1 = 65;
    
    TBCTL = TBSSEL_1 + MC_1; //Timer_B ACLK y en modo Up to TBCCR0 
    TBCCR0 = 70-1;  //50hz
    display_set_pos(0x40);
    printf("A: Modo timer");
    display_set_pos(0x14);
    printf("B: Modo Sensor");
    display_set_pos(0x54);
    printf("C: Modo IDLE");
           
    //compuerta_servo(compuerta_ctl);
    _BIS_SR(LPM0_bits + GIE);
    
}

#define CONV(x)      x			// convertir x en voltaje

// Timer A0 rutina de servicio de interrupción
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
 // if(modo_sensor == 1) { 
   //   if ((tiempo_alcanzado == 1) && (ya_iniciado ==1)){
    //    adc_single_channel_single_conv_it(0);
     //   res = _resultado();
  
  if(contador_reloj==50)
  {
    while(i2c_is_busy());
    i2c_master_read_it(0x00,buffer,7);
    //display_set_pos(0x00);
    //printf("%02x:%02x:%02x  %02x/%02x/20%02x",buffer[2],buffer[1],buffer[0],buffer[4],buffer[5],buffer[6]);
    contador_reloj = 0;
  }
  if(modo==2)
  {
  //  cat_detected = detectado();
       if ((tiempo_alcanzado == 1) && (gato_detectado == 0)){
          P5OUT &= ~BIT5;
          adc_init_single_channel_single_conv_it();
          adc_single_channel_single_conv_it(0);
       //   res = _resultado();
        }
        else {
          P5OUT |= BIT5;
         // adc_single_channel_single_conv_it(0);
        }
  }
  contador_reloj+=1;
  
    //CONDICIONES DE TIEMPO ALCANZADO
  if(buffer[4]!=0) {
    if ((buffer[2]==hora1_bcd) && (buffer[1]==min1_bcd) && (buffer[0]==0x00)){
      tiempo_alcanzado = 1;
    }
    else if ((buffer[2]==hora2_bcd) && (buffer[1]==min2_bcd) && (buffer[0]==0x00)){
      tiempo_alcanzado = 1;
    }
    else if ((buffer[2]==hora3_bcd) && (buffer[1]==min3_bcd) && (buffer[0]==0x00)){
      tiempo_alcanzado = 1;
    }
  }
}

#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
 // cat_detected = detectado();
  if ((tiempo_alcanzado==1) && (modo == 1)){
    compuerta_servo(1);
    P5OUT &= ~BIT3;
    P5OUT |= BIT4;
    control_steper();
    contador_main +=1;
    if (contador_main == 5000){
      contador_main = 0;
      P2OUT &= ~0x17;
      compuerta_servo(0);
      tiempo_alcanzado = 0;
      P5OUT &= ~BIT4;
      P5OUT |= BIT3;
    }
  }  
  
  else if ((gato_detectado == 1) && (modo == 2) && (tiempo_alcanzado==1)){
  //  compuerta_servo(1);
    control_steper();
    contador_main += 1;
    if (contador_main == 5000) {
      contador_main=0;
      P2OUT &= ~0x17;
      ADC12IE |= 0x01;
      tiempo_alcanzado = 0;
      compuerta_servo(0);
      P5OUT &= ~BIT4;
      P5OUT |= BIT3;
      gato_detectado = 0;
    }
  }
}


