#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define LED_RED BIT6             // P1.6


short redrawScreen = 1;
u_int fontFgColor = COLOR_GREEN;
u_char helloCol = 10;
u_char nextHelloCol = 10;
signed char  helloVelocity = 1;

void wdt_c_handler()
{
  static int secCount = 0;
  static int dsecCount = 0;
  secCount ++;
  dsecCount ++;
  if (secCount == 250) {		/* once/sec */
    secCount = 0;
    fontFgColor = (fontFgColor == COLOR_GREEN) ? COLOR_BLACK : COLOR_GREEN;
    redrawScreen = 1;
  }
  if(dsecCount == 25){
    dsecCount = 0;
    nextHelloCol += helloVelocity;
    if(nextHelloCol > 30   || nextHelloCol < 10) {
      helloVelocity = -helloVelocity;
      nextHelloCol += helloVelocity;
    }
    redrawScreen =1;
  }
}
  

void main()
{
  P1DIR |= LED_RED;		/**< Green led on when CPU on */		
  P1OUT |= LED_RED;
  configureClocks();
  lcd_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(COLOR_BLUE);
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      drawString5x7(helloCol,20,"hello", fontFgColor, COLOR_BLUE); // this is the inital move
      drawString5x7(nextHelloCol, 20, "hello", fontFgColor, COLOR_BLUE); // this is the next move
      helloCol = nextHelloCol;
    }
    P1OUT &= ~LED_RED;	/* green off */
    or_sr(0x10);		/**This is where the CPU will go to sleep */
    P1OUT |= LED_RED;		/* green on */
  }
}

    
    



