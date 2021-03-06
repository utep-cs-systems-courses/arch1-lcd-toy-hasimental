#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "p2switches.h"
#include <sr.h>

#define LED_GREEN BIT6             // P1.6


short redrawScreen = 1;
u_int fontFgColor = COLOR_GREEN;
u_char start = 10;
u_char nextSpace = 10;
signed char velocity = 30;

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
    nextSpace += velocity;
    if(nextSpace > 150 || nextSpace < 0){
      velocity = -velocity;
      nextSpace += velocity;
    }
    redrawScreen =1;
  }
}
// here is method to draw the doorway passing the row, column, radius, and color of the circle
void drawDoorway(u_char row, u_char col,int radius, u_int colorBGR){
  int diameter = 2*radius; 
  int x;
  int y;
  for(int i = 0; i < diameter+1; i++){
    for(int j = 0; j < diameter+1; j++){
      x = i - radius;
      y = j - radius;
      /*This is the for to draw the circle*/
      if(((x*x) + (y*y)) <= (radius*(radius+1)))
	drawPixel(i+row,j+col, colorBGR); // here we add row and column so that you could move it 
      else
	drawPixel(i-row,j-col, COLOR_BLACK);
    }
  }
  /*This is the for loops to draw the square */
  /*we add 1 to i and j so that we it could align with the size of the circle and we add 10 to the column because that lowers the square where it meets halfway through the cirlce*/
  for(int i = 0; i < diameter+1; i++){
    for(int j = 0; j < diameter+1; j++){
      drawPixel(i+row,j+col+10,colorBGR);
    }
   }
}


void main()
{
  P1DIR |= LED_GREEN;		/**< Green led on when CPU on */		
  P1OUT |= LED_GREEN;
  configureClocks();
  lcd_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(COLOR_BLACK);
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      int x = 50;
      int y = 20;
      int circleRadius = 10;
     
      /* this is if you want to make a pixel up and down which you could switch the helloCol and next Col to be in the row and that would make it move from left to right instead of up and down*/ 
      // drawPixel(10,helloCol, COLOR_BLUE);
      // drawPixel(10, nextHelloCol, COLOR_WHITE);
      
      /* this is where we draw the circle and make it look like it is moving up and down*/
      /* parameters (x,y,radius, color)*/
      drawDoorway(x,start,circleRadius,COLOR_BLACK);
      drawDoorway(x,nextSpace,circleRadius, COLOR_WHITE);
      
      /*here is if you want the string to move across the screen*/
      // drawString5x7(helloCol,75, "hello", COLOR_BLUE, COLOR_BLUE);
      // drawString5x7(nextHelloCol,75, "hello", fontFgColor, COLOR_BLUE);
      start = nextSpace;
    }
    P1OUT &= ~LED_GREEN;	/* green off */
    or_sr(0x10);		/**< CPU OFF */
    P1OUT |= LED_GREEN;		/* green on */
  }
}



    



