#ifndef LEDDRIVER
#define LEDDRIVER
#include "Adafruit_DotStar.h"
Adafruit_DotStar strip = Adafruit_DotStar(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

enum LEDSTATE
{
  RED,
  GREEN,
  BLUE,
  WHITE,
  GAMING
};
char LedState = 0;

void ledGaming(void)
{
  static char R = 64;
  static char G = 16;
  static char B = 0;
  static char state = 0;
  static long Time = 0;

  if(millis()- Time > 50)
  {
    Time = millis();
    switch(state)
    {
      case(0):
        R--;G++;if(R==0)state++;
        break;
        
      case(1):
        G--;B++;if(G==0)state++;
        break;
        
      case(2):
        B--;R++;if(B==0)state=0;
        break;
    }
    strip.setPixelColor(0, R, G, B);
  }
}

void ledDriver(void)
{
  static char BeatHeart = 0;
  static char State = 0;
  static long Time = 0;

//BeatHeart management 
  if(millis()- Time > 50)
  {
    Time = millis();
    if(State == 0)
    {
      BeatHeart ++;
      if(BeatHeart == 32 )State = 1;
    }
    else
    {
      BeatHeart --;
      if(BeatHeart == 0 )State = 0;
    }
  }
//Appli show 
  switch(LedState)
  {
     case(RED):
      strip.setPixelColor(0, BeatHeart, 0, 0);
      break;

    case(GREEN):
      strip.setPixelColor(0, 0, BeatHeart, 0);
      break;
      
    case(BLUE):
      strip.setPixelColor(0, 0, 0, BeatHeart);
      break;
      
     case(WHITE):
      strip.setPixelColor(0, 255, 255, 255);
      break;
      
    case(GAMING):
      ledGaming();
      break;
  }
  strip.show();
}
#endif
