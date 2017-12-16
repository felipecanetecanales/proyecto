#include <stdio.h>
#include "display.h"
#include "keyboard.h"
/***************************************************
* Nombre del modulo: keyboard.c
*
* Modulo creado para la asignatura Elo312
* Laboratorio de Estructura de Computadores
* del departamento de Electrónica de la Universidad
* Técnica Federico Santa María. El uso o copia
* está permitido y se agracede mantener el nombre 
* de los creadores.
*
* Escrito inicialmente el 01/01/2004 Por Michael Kusch & Wolfgang Freund
* Modificado el 24/09/2014           Por Mauricio Solís & Andrés Llico.
*
* Descripción del módulo:
* Modulo driver teclado matricial
* Contiene las funciones que permiten manejar el
* teclado matricial.
***************************************************/

/*  Include section
*
***************************************************/
#include "keyboard.h"
char arreglo2[]={0,0,0,0,0,0,0};
int datitos[]={0,0,0,0,0,0,0,};
int taccr[]={0,0,0,126};
int actividad=0;
int contador;
int numero = 0;
int p_anterior =0;
int c=0;
int j=0;
int k =0;
int num=0;
char modo=0;
char minuto_actual = 0;
char minuto_actual2 = 0;
char minuto_actual3 = 0;
char minuto_antigua = 0;
char minuto_antigua2 = 0;
char minuto_antigua3 = 0;
char hora_actual = 0;
char hora_actual2 = 0;
char hora_actual3 = 0;
char hora_antigua = 0;
char hora_antigua2 = 0;
char hora_antigua3 = 0;
char hora1_bcd = 0;
char hora2_bcd = 0;
char hora3_bcd = 0;
char min1_bcd = 0;
char min2_bcd = 0;
char min3_bcd = 0;

/*  Defines section
*
***************************************************/
typedef unsigned char uchar;

/*  Local Function Prototype Section
*
***************************************************/
void keyboard_button_action(char);


/*  Global Variables Section
*
***************************************************/
uchar BUTMAT_CLK = 0;           // cuántas interrupciones se lleva presionado el botón
uchar BUTMAT_PREV = 0xFF;       // tecla de la lectura anterior, FF => ninguna.


////////////////////////////////////////
//       RUTINAS IMPLEMENTADAS        //
//                                    //

/**************************************************
* Nombre    		: void keyboard_init(void)
* returns			: void
* Creada por		: Michael Kusch
* Fecha creación	: 01/01/2004
* Descripción		: Inicializa los puertos del 
* driver del teclado.
**************************************************/
void keyboard_init(void)
{
    PTOUT = 0x00;
    PTDIR = 0x00;
}

/**************************************************
* Nombre    		: void keyboard_tec_matrix(void)
* returns			: void
* Creada por		: Michael Kusch
* Fecha creación	: 01/01/2004
* Descripción		: Lee la matriz de botones y 
* llama a la función keyboard_button_action() cuando
* detecta que se ha presionado una tecla (o se ha mantenido
* presionada una tecla).
* Esta función debe ser llamada cada cierta cantidad de tiempo.
**************************************************/
void keyboard_tec_matrix(void)
{
    uchar BUTMAT = 0xFF;        // tecla de 0 a 15
    uchar k, temp;

    temp = 0x00;
    for(PTDIR = 0x10; PTDIR > 0; PTDIR <<= 1) {
        asm("NOP"); asm("NOP");
        asm("NOP"); asm("NOP");
        asm("NOP");
        k = (((~PTIN) & 0x0F) | PTDIR);
        if ( (k & 0x0F) )	           // si hay algún bit de columna en 0, entonces if es verdadero
            temp = k;	               // y se guarda el valor de la columna y filas activas
    }

    if (temp) {	                       // hubo tecla presionada?
        switch	(temp & 0xF0) {
        case BIT4:
            BUTMAT = 0;	 // fila 1
            break;
        case BIT5:
            BUTMAT = 4;	 // fila 2
            break;
        case BIT6:
            BUTMAT = 8;	 // fila 3
            break;
        case BIT7:
            BUTMAT = 12; // fila 4
            break;
        }

        switch	(temp & 0x0F) {
        case BIT0:
            asm("NOP");
            BUTMAT += 0; // col 1
            break;
        case BIT1:
            BUTMAT += 1; // col 2
            break;
        case BIT2:
            BUTMAT += 2; // col 3
            break;
        case BIT3:
            BUTMAT += 3; // col 4
            break;
        }

        if (BUTMAT != BUTMAT_PREV) {              // botón presionado es distinto al anterior?
            BUTMAT_PREV = BUTMAT;
            BUTMAT_CLK = 0;
            keyboard_button_action(BUTMAT);
        }
        else {
            if (++BUTMAT_CLK == 100) {            // presionada por más de 100*10[ms] (1 [s])?
                keyboard_button_action(BUTMAT);   // actuar como si se volvió a presionar el botón
                BUTMAT_CLK -= 33;                 // por cada 0.5 [s] que siga presionado el botón
            }
        }
    }
    else
        BUTMAT_PREV = 0xFF;                       // ningún botón presionado
}
//                                    //
////////////////////////////////////////


////////////////////////////////////////
//     RUTINAS NO IMPLEMENTADAS       //
//                                    //
/**************************************************
* Nombre    		: void tecla_to_number(char arg1)
* returns			: void
* arg1				: Tecla presionada
* Creada por		: Felipe Cañete
* Fecha creación	: 07/12/2017
* Descripción		: Transforma la tecla presionada a
numero entero
**************************************************/
int tecla_to_number(char tecla)
{
  switch (tecla){
  case 0:
    numero = 1;
    break;
  case 1:
    numero = 2;
    break;
  case 2:
    numero = 3;
    break;
  case 4: 
    numero = 4;
    break;
  case 5: 
    numero = 5;
    break;
  case 6:
    numero = 6;
    break;
  case 8:
    numero = 7;
    break;
  case 9:
    numero = 8;
    break;
  case 10:
    numero = 9;
    break;
  case 13:
    numero = 0;
    break;
  }
  return numero;
}
/**************************************************
* Nombre    		: void send_to_array(int arg1,int arg2, int arg3,
                          int arg5, int arg6, int arg7)
* returns			: void
* arg1				: arreglo con horas y minutos
* Creada por		: Felipe Cañete
* Fecha creación	: 07/12/2017
* Descripción		: Ordena el arreglo de menor hora a mayor y retorna 
                          un arreglo con todos los datos mas el tipo de modo
**************************************************/
/*int send_to_array(int act, int* hour1, int hour2, int hour3, int min1, int min2, int min3){
  arreglo2[0] = act; //en el primer dato del arreglo se guarda el tipo de modo
  //DEFINIMOS EL ORDEN DE LAS HORAS Y SUS MINUTOS ARREGLO[]={MODO,HORA1,HORA2,HORA3,MIN1,MIN2,MIN3}
  if (hour1 >= hour2){
    if(hour1 >= hour3){
      arreglo2[3]=hour1; // hour1 seria la mayor hora
      arreglo2[6]=min1;  // asignamos su minuto correspondiente
      if (hour2 >= hour3){
        arreglo2[2]=hour2; //hour2 seria la hora del medio
        arreglo2[5]=min2;
        arreglo2[1]=hour3; //hour3 seria la menor hora
        arreglo2[4]=min3;
      }
      else{
        arreglo2[2]=hour3; //hour3 seria la hora del medio
        arreglo2[5]=min3;
        arreglo2[1]=hour2; //hour2 seria la menor hora
        arreglo2[4]=min2;
      }
    }
    else{
      arreglo2[3]=hour3; //como hour1 era mayor a hour2, pero no es mayor a hour3, hour3 es la mayor
      arreglo2[6]=min3;
      arreglo2[2]=hour1; //hour1 quedaria como la hora del medio
      arreglo2[5]=min1;
      arreglo2[1]=hour2; //hour2 seria la menor hora
      arreglo2[4]=min2;
    }
  }
  else if (hour2 > hour3){
    arreglo2[3]=hour2; //hour2 seria la mayor hora
    arreglo2[6]=min2;
    if(hour3 >= hour1){
      arreglo2[2]=hour3; //hour3 seria la hora del medio
      arreglo2[5]=min3;
      arreglo2[1]=hour1; //hour1 seria la menor hora
      arreglo2[4]=min1;
    }
    else{
      arreglo2[2]=hour1; //hour1 seria la hora del medio
      arreglo2[5]=min1;
      arreglo2[1]=hour3; //hour3 seria la menor hora
      arreglo2[4]=min3;
    }
  }
 return *arreglo2;
}*/

/**************************************************
* Nombre    		: char dec_to_bcd(char num)
* returns			: void
* arg1				: arreglo con horas y minutos
* Creada por		: Felipe Cañete
* Fecha creación	: 07/12/2017
* Descripción		: convierte el decimal a bcd
**************************************************/
char dec_to_bcd(char num){
  char bcd1 = 0;
  char bcd2 = 0;
  bcd1 = (num/10) << 4;
  bcd2 = (num%10);
  return bcd1 + bcd2;
}

/**************************************************
* Nombre    		: void keyboard_button_action(char arg1)
* returns			: void
* arg1				: Tecla presionada
* Creada por		: Michael Kusch
* Fecha creación	: 01/01/2004
* Descripción		: Realiza una accion segun la
tecla que ha sido presionada.
**************************************************/
void keyboard_button_action(char tecla)
{
  //////EDITOR 1 ///////
switch (tecla){         //definiremos los modos de muestreo mediante la eleccion de las letras A,B,C,D
case 3:{                // en caso de apretar tecla A activa la opcion de entregar comida cada 3 horas
    if(p_anterior != tecla){
      display_set_pos(0x14);
      printf("Modo Timer activado");
      actividad = 1;
      display_clear();
      display_set_pos(0x14);
      printf("Ingrese Hora 1");
      c = 0;
      modo = 1;
      break;
    }
    else{
      display_set_pos(0x14);
      printf("Modo ya esta en uso");
      break;
    }
}
case 7:{  //Modo intervalo de tiempo definidos manualmente
    if(p_anterior != tecla){
      display_set_pos(0x14);
      printf("Modo Sensor");
      display_set_pos(0x44);
      printf("activado");
      actividad = 2;
      modo = 2;
      c=0;
      while(j<10000){
        j++;
      }
      display_clear();
      display_set_pos(0x14);
      printf("Ingrese Hora 1");
      
      break;
    }
    else{
      display_set_pos(0x14);
      printf("modo ya esta en uso");
    }
}
case 11:{ //Modo IDLE
      display_clear();
      modo =3;
}
default:{
    break;
  }

}

switch (modo){
case 1:{
        modo = 1;
        switch (c){
        case 0:{
          if ((tecla != 0x03)){
            hora_actual = tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%d0:00",hora_actual);
            c=1;
            break;
          }
          else {
            c=0;
            break;
          }
        }
        case 1:{
          if (tecla != 15){
            hora_actual = hora_antigua*10 + tecla_to_number(tecla); //observar con el WATCH si se suma bien a la hora_actual
            display_set_pos(0x54);
            printf("%02d:00",hora_actual);
            c=2;
            break;
          }
          else{
            c=1;
          }
          break;
        }
        case 2:{
          if (tecla != 15){
            minuto_actual = tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:%d0",hora_antigua,minuto_actual);
            c=3;
            break;
          }
          else{
            c=2;
          }
          break;
        }
        case 3:{
          if (tecla != 15){
            minuto_actual = minuto_antigua*10 + tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:%02d",hora_antigua,minuto_actual);
            c=4;
            display_set_pos(0x14);
            printf("Ingrese Hora 2");
            break;
          }
          else{
            c=3;
          }
          break;
        }
        case 4:{ //Comienzo de segundo intervalo de tiempo, primer digito de la hora
          if (tecla != 15){
            hora_actual2 = tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%d0:00",hora_actual2);
            c=5;
            break;
          }
          else{
            c=4;
          }
          break;
        }
        case 5:{ //segundo dígito de la segunda hora
          if (tecla != 15){
            hora_actual2 = hora_antigua2*10 + tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:00",hora_actual2);
            c=6;
            break;
          }
          else{
            c=5;
          }
          break;
        }
        case 6:{
          if(tecla!=15){
            minuto_actual2 = tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:%d0",hora_antigua2,minuto_actual2);
            c=7;
            break;
          }
          else{
            c=6;
          }
        }
        case 7:{
          if(tecla!=15){
            minuto_actual2 = minuto_antigua2*10 + tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:%02d",hora_antigua2,minuto_actual2);
            c=8;
            display_set_pos(0x14);
            printf("Ingrese Hora 3");
            break;
          }
          else {
            c=9;
          }
        }
        case 8:{
          if (tecla != 15){
            hora_actual3 = tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%d0:00",hora_actual3);
            c=9;
            break;
          }
          else{
            c=8;
          }
          break;
        }
        case 9:{
          if (tecla != 15){
            hora_actual3 = hora_antigua3*10 + tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:00",hora_actual3);
            c=10;
            break;
          }
          else{
            c=9;
          }
          break;
        }
        case 10:{
          if (tecla != 15){
            minuto_actual3 = tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:%d0",hora_antigua3,minuto_actual3);
            c=11;
            break;
          }
          else{
            c=10;
          }
          break;
        }
        case 11:{
          if (tecla != 15){
            minuto_actual3 = minuto_antigua3*10 + tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:%02d",hora_antigua3,minuto_actual3);
            c=12;
            break;
          }
          else{
            c=11;
          }
          break;
        }
        case 12:{
          if (tecla == 15){
             display_clear();
             display_set_pos(0x40);
             printf("Horas       1)%02d:%02d",hora_antigua,minuto_antigua);
             display_set_pos(0x20);
             printf("2)%02d:%02d",hora_antigua2,minuto_antigua2);
             display_set_pos(0x60);
             printf("3)%02d:%02d",hora_antigua3,minuto_antigua3);
            // *datitos=send_to_array(1,&hora_antigua,hora_antigua2,hora_antigua3,minuto_antigua,minuto_antigua2,minuto_antigua3);             
  //la idea de la funcion send_to_array es lograr obtener el arreglo ordenado, 
  // lo instanciamos desde el main para poder operar con las horas,
  // asi podremos comprar la hora del real time clock con la de los intervalos
  // del arreglo.             
          }
          else{
            c=12;
          }
        }
      }
        if (tecla == 15){
          c+=1;
        }
        hora_antigua = hora_actual;
        minuto_antigua = minuto_actual;
        hora_antigua2 = hora_actual2;
        minuto_antigua2 = minuto_actual2;
        hora_antigua3 = hora_actual3;
        minuto_antigua3 = minuto_actual3;
        hora1_bcd = dec_to_bcd(hora_antigua);
        hora2_bcd = dec_to_bcd(hora_antigua2);
        hora3_bcd = dec_to_bcd(hora_antigua3);
        min1_bcd  = dec_to_bcd(minuto_antigua);
        min2_bcd  = dec_to_bcd(minuto_antigua2);
        min3_bcd  = dec_to_bcd(minuto_antigua3); 
        break;
}
case 2:{
        modo = 2;
        switch (c){
        case 0:{
          if ((tecla!=0x07)){
            hora_actual = tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%d0:00",hora_actual);
            c=1;
            break;
          }
          else {
            c=0;
            break;
          }
        }
        case 1:{
          if (tecla != 15){
            hora_actual = hora_antigua*10 + tecla_to_number(tecla); //observar con el WATCH si se suma bien a la hora_actual
            display_set_pos(0x54);
            printf("%02d:00",hora_actual);
            c=2;
            break;
          }
          else{
            c=1;
          }
          break;
        }
        case 2:{
          if (tecla != 15){
            minuto_actual = tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:%d0",hora_antigua,minuto_actual);
            c=3;
            break;
          }
          else{
            c=2;
          }
          break;
        }
        case 3:{
          if (tecla != 15){
            minuto_actual = minuto_antigua*10 + tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:%02d",hora_antigua,minuto_actual);
            c=4;
            display_set_pos(0x14);
            printf("Ingrese Hora 2");
            break;
          }
          else{
            c=3;
          }
          break;
        }
        case 4:{ //Comienzo de segundo intervalo de tiempo, primer digito de la hora
          if (tecla != 15){
            hora_actual2 = tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%d0:00",hora_actual2);
            c=5;
            break;
          }
          else{
            c=4;
          }
          break;
        }
        case 5:{ //segundo dígito de la segunda hora
          if (tecla != 15){
            hora_actual2 = hora_antigua2*10 + tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:00",hora_actual2);
            c=6;
            break;
          }
          else{
            c=5;
          }
          break;
        }
        case 6:{
          if(tecla!=15){
            minuto_actual2 = tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:%d0",hora_antigua2,minuto_actual2);
            c=7;
            break;
          }
          else{
            c=6;
          }
        }
        case 7:{
          if(tecla!=15){
            minuto_actual2 = minuto_antigua2*10 + tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:%02d",hora_antigua2,minuto_actual2);
            c=8;
            display_set_pos(0x14);
            printf("Ingrese Hora 3");
            break;
          }
          else {
            c=9;
          }
        }
        case 8:{
          if (tecla != 15){
            hora_actual3 = tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d0:00",hora_actual3);
            c=9;
            break;
          }
          else{
            c=8;
          }
          break;
        }
        case 9:{
          if (tecla != 15){
            hora_actual3 = hora_antigua3*10 + tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:00",hora_actual3);
            c=10;
            break;
          }
          else{
            c=9;
          }
          break;
        }
        case 10:{
          if (tecla != 15){
            minuto_actual3 = tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:%d0",hora_antigua3,minuto_actual3);
            c=11;
            break;
          }
          else{
            c=10;
          }
          break;
        }
        case 11:{
          if (tecla != 15){
            minuto_actual3 = minuto_antigua3*10 + tecla_to_number(tecla);
            display_set_pos(0x54);
            printf("%02d:%02d",hora_antigua3,minuto_actual3);
            c=12;
            break;
          }
          else{
            c=11;
          }
          break;
        }
        case 12:{
          if (tecla == 15){
             display_clear();
             display_set_pos(0x40);
             printf("Horas       1)%02d:%02d",hora_antigua,minuto_antigua);
             display_set_pos(0x20);
             printf("2)%02d:%02d",hora_antigua2,minuto_antigua2);
             display_set_pos(0x60);
             printf("3)%02d:%02d",hora_antigua3,minuto_antigua3);            
          }
          else{
            c=12;
          }
        }
      }
        if (tecla == 15){
          c+=1;
        }
        hora_antigua = hora_actual;
        minuto_antigua = minuto_actual;
        hora_antigua2 = hora_actual2;
        minuto_antigua2 = minuto_actual2;
        hora_antigua3 = hora_actual3;
        minuto_antigua3 = minuto_actual3;
        hora1_bcd = dec_to_bcd(hora_antigua);
        hora2_bcd = dec_to_bcd(hora_antigua2);
        hora3_bcd = dec_to_bcd(hora_antigua3);
        min1_bcd  = dec_to_bcd(minuto_antigua);
        min2_bcd  = dec_to_bcd(minuto_antigua2);
        min3_bcd  = dec_to_bcd(minuto_antigua3);
        break;
}
case 3:{
  display_clear();
  display_set_pos(0x14);
  printf("     MODO IDLE     ");
  break;
}
}
}

      /*arreglo2[0]=actividad;
      arreglo2[1]=hora_antigua;
      arreglo2[2]=minuto_antigua;*/