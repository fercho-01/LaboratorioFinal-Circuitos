/*

 TFT Pong

 This example for the Arduino screen reads the values
 of 2 potentiometers to move a rectangular platform
 on the x and y axes. The platform can intersect
 with a ball causing it to bounce.

 This example code is in the public domain.

 Created by Tom Igoe December 2012
 Modified 15 April 2013 by Scott Fitzgerald

 http://www.arduino.cc/en/Tutorial/TFTPong

 */

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

#define TFT_CS     10
#define TFT_RST    9 
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

#define TFT_SCLK 13   
#define TFT_MOSI 11   
#define SD_CS    4  // Chip select line for SD card

// variables for the position of the ball and paddle
int paddle1 = 0;
int paddle2 = 0;
int oldPaddle1, oldPaddle2;
int ballDirectionX = 1;
int ballDirectionY = 1;

int ballSpeed = 10; // lower numbers are faster

int ballX, ballY, oldBallX, oldBallY;

int pointP1=0, pointP2=0;
  int myWidth = tft.width();
  int myHeight = tft.height();
  bool point=false;

void setup() {
  Serial.begin(9600);
  // initialize the display
  tft.initR(INITR_BLACKTAB);      
  // black background
  tft.background(0, 0, 0);
}

void loop() {

  // save the width and height of the screen

  // map the paddle's location to the position of the potentiometers
  paddle1 = map(analogRead(A0), 0, 1024, 0, myWidth-20) ;
  paddle2 = map(analogRead(A1), 0, 1024, 0, myWidth-20);

  


  //Borramos la posicion anterior de la barrita
  tft.fill(0, 0, 0);

  if (oldPaddle1 != paddle1) {
    tft.rect(oldPaddle1, 0, 20, 5);
    point=false;
  }
    if (oldPaddle2 != paddle2) {
    tft.rect(oldPaddle2, myHeight-5, 20, 5);
    point=false;
  }

//dibujamos la nueva posicion de la barrita
  tft.fill(255, 0, 0);

  tft.rect(paddle1, 0, 20, 5);
  oldPaddle1 = paddle1;
  tft.fill(0, 255, 0);
  tft.rect(paddle2, myHeight-5, 20, 5);
  oldPaddle2 = paddle2;

  // update the ball's position and draw it on screen
  if (millis() % ballSpeed < 2&& !point) {
    moveBall();
  }
}

// this function determines the ball's position on screen
void moveBall() {
  // if the ball goes offscreen, reverse the direction:
  if (ballX > myWidth || ballX < 0) {
    ballDirectionX = -ballDirectionX;
  }

  if (ballY > myHeight ) {
    //ballDirectionY = -ballDirectionY;
    pointP1++;
      ballX = myWidth/2;
      ballY = myHeight/2;
      point=true;
  }else if(ballY < 0) {
    pointP2++;
    ballX = myWidth/2;
    ballY = myHeight/2;
    point=true;
  }

  // check if the ball and the paddle occupy the same space on screen
  if (inPaddle(ballX, ballY, paddle1, 0, 20, 5)||inPaddle2(ballX, ballY, paddle2, tft.height()-5, 20, -5)) {
    ballDirectionX = -ballDirectionX;
    ballDirectionY = -ballDirectionY;
  }

  // update the ball's position
  ballX += ballDirectionX;
  ballY += ballDirectionY;

  // erase the ball's previous position
  tft.fill(0, 0, 0);

  if (oldBallX != ballX || oldBallY != ballY) {
    tft.rect(oldBallX, oldBallY, 5, 5);
  }


  // draw the ball's current position
  tft.fill(255, 255, 255);
  tft.rect(ballX, ballY, 5, 5);

  oldBallX = ballX;
  oldBallY = ballY;

}

// this function checks the position of the ball
// to see if it intersects with the paddle
boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {
  boolean result = false;

  if ((x >= rectX && x <= (rectX + rectWidth)) &&
      (y >= rectY && y <= (rectY + rectHeight))) {
    result = true;
  }

  return result;
}
boolean inPaddle2(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {
  boolean result = false;

  if ((x >= rectX && x <= (rectX + rectWidth)) &&
      (y <= rectY && y >= (rectY + rectHeight))) {
    result = true;
  }

  return result;
}
