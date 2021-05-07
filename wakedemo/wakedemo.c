#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define LED_RED BIT6             // P1.6


short redrawScreen = 1;
u_int fontFgColor = COLOR_GREEN;
u_char helloCol = 10;
u_char nextHelloCol = 10;
signed char  helloVelocity = 30;

void wdt_c_handler()
{
  static int secCount = 0;
  static int dsecCount = 0;
  secCount ++;
  dsecCount ++;
  if (secCount == 250) {		/* once/sec */
    secCount = 0;
    fontFgColor = (fontFgColor == COLOR_GREEN) ? COLOR_BLUE : COLOR_GREEN; // changed the color to blue so that it you cannot see it at first
    redrawScreen = 1;
  }
  // this is to make it animated moving from left to right and to be used in the main
  // you could use switch where you put the helloCol and nextHelloCol to make it move  up and down
  // also here you could change the varible helloVelocity to make it move faster
  if(dsecCount == 250){
    dsecCount = 0;
    nextHelloCol += helloVelocity;
    if(nextHelloCol > 150 || nextHelloCol < 0){
      helloVelocity = -helloVelocity;
      nextHelloCol += helloVelocity;
    }
    redrawScreen =1;
  }
}
// here is method to draw a circle passing the row, column, radius, and color of the circle
void drawCircle(u_char row, u_char col,int radius, u_int colorBGR){
  int diameter = 2*radius; 
  int x;
  int y;
  for(int i = 0; i < diameter+1; i++){
    for(int j = 0; j < diameter+1; j++){
      x = i - radius;
      y = j - radius;

      if(((x*x) + (y*y)) <= (radius*(radius+1)))
	drawPixel(i+row,j+col, colorBGR); // here we add row and column so that you could move it 
      else
	drawPixel(i-row,j-col, COLOR_BLUE);
    }
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
  
  clearScreen(COLOR_BLACK);
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      /* this is if you want to make a pixel up and down which you could switch the helloCol and next Col to be in the row and that would make it move from left to right instead of up and down*/ 
      // drawPixel(10,helloCol, COLOR_BLUE);
      // drawPixel(10, nextHelloCol, COLOR_WHITE);
      
      /* this is where we draw the circle and make it look like it is moving up and down*/
      drawCircle(50,helloCol,10,COLOR_BLACK);
      drawCircle(50 ,nextHelloCol,10, COLOR_WHITE);

      /*here is if you want the string to move across the screen*/
      // drawString5x7(helloCol,75, "hello", COLOR_BLUE, COLOR_BLUE);
      // drawString5x7(nextHelloCol,75, "hello", fontFgColor, COLOR_BLUE);
      helloCol = nextHelloCol;
    }
    P1OUT &= ~LED_RED;	/* green off */
    or_sr(0x10);		/**< CPU OFF */
    P1OUT |= LED_RED;		/* green on */
  }
}



    



