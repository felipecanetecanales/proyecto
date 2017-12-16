#ifndef __button_matrix
#define __button_matrix
/*  Include section
*
***************************************************/
#include "msp430_version.h"

/*  Defines section
*
***************************************************/
#define PTOUT 			P1OUT
#define PTIN 			P1IN
#define PTDIR 			P1DIR

/*
BIT		7	6	5	4	3	2	1	0	
TEC		F4	F3	F2	F1	C4	C3	C2	C1
*/

/*  Function Prototype Section
*
***************************************************/
void keyboard_init(void);
void keyboard_tec_matrix(void);
int tecla_to_number(char);
int send_to_array(int, int, int, int, int, int, int);

#endif
