#ifndef DISPLAY__H
#define DISPLAY__H
/*  Include section
*
***************************************************/
#include "msp430_version.h"

/*  Defines section
*
***************************************************/
// defines display layout
#define LCD_LINES			4
#define LCD_WIDTH			20

// defines for data
#define	LDOUT			P4OUT	// dataport
#define LDIN			P4IN
#define	LDSEL			P4SEL
#define	LDDIR			P4DIR

// defines for control
#define LCOUT			P5OUT
#define	LCDIR			P5DIR
#define LCSEL			P5SEL
#define	LCD_RS			BIT0	// bit meaning: 1 = Data , 0 = Control
#define LCD_RW			BIT1	// bit meanings: 0 = Write, 1 = Read
#define	LCD_E			BIT2	// Enable

/*  Function Prototype Section
*
***************************************************/

void display_clear(void);
void display_right_shift(void);
void display_left_shift(void);
void display_new_character(int, char*);
unsigned char display_get_pos(void);
void display_init(void);
void display_set_pos(unsigned char);
void display_lcd_clear(int, int);
void display_send_cmd(unsigned char);
void display_send_data(char);
void display_test_Write_CGRAM_MS(void);

#endif
