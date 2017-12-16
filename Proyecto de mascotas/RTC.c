#include "RTC.h"
#include "display.h"
#include <stdio.h>
extern char buffer[10];

void RTC_print()
{
  display_set_pos(0x00);
  printf("%02x:%02x:%02x  %02x/%02x/20%02x",buffer[2],buffer[1],buffer[0],buffer[4],buffer[5],buffer[6]);
}