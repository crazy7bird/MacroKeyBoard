#include "Adafruit_DotStar.h"
//#include <Keyboard.h>
// include the HID library
#include "HID-Project/HID-Project.h"
Adafruit_DotStar strip = Adafruit_DotStar(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

enum MODE
{
  Red,
  Media, 
  Blue,
  Pink
};
int Mode = 0;

enum PRESSTYPE
{
  TAP,
  DOUBLETAP,
  TRIPLETAP,
  LONG
};


void Keypress(int Key, int pressType)
{
  switch(Mode)
  {
    case(Red):
      if(Key == 3)
      {
        if(pressType == TAP){Mode ++;strip.setPixelColor(0, 0, 16, 0); strip.show();} //LED GREEN for next mode.
      }
      break;
      
    case(Media):
      if(Key==0)
      {
        if(pressType == TAP)Consumer.write(MEDIA_NEXT);
        if(pressType == LONG)Consumer.write(MEDIA_VOLUME_UP);
      }
      if(Key==1)
      {
        if(pressType == TAP)Consumer.write(MEDIA_PREV);
        if(pressType == LONG)Consumer.write(MEDIA_VOLUME_DOWN);
      }
      if(Key==2)
      {
        if(pressType == TAP)Consumer.write(MEDIA_PLAY_PAUSE);
      }
      if(Key == 3)
      {
        if(pressType == TAP){Mode ++;strip.setPixelColor(0, 0, 0, 16); strip.show();} //LED BLUE for next mode.
      }
      break;
      
     case(Blue):
      if(Key == 2)
      {
        if(pressType == TAP){Keyboard.press(KEY_LEFT_WINDOWS);Keyboard.print("l");}
      }
      if(Key == 3)
      {
        if(pressType == TAP){Mode ++;strip.setPixelColor(0, 16, 5, 9); strip.show();} //LED PINK for next mode.
      }
      break;
      
     case(Pink):
      //Test TAP TYPE
      if(Key == 0)
      {
        if(pressType == TAP)strip.setPixelColor(0, 16, 0, 16);
        if(pressType == DOUBLETAP)strip.setPixelColor(0, 16, 16, 0);
        if(pressType == TRIPLETAP)strip.setPixelColor(0, 255, 255, 255);
        if(pressType == LONG)strip.setPixelColor(0, 0, 16, 16);
        strip.show();
      }
      if(Key == 3)
      {
        if(pressType == TAP){Mode =0;strip.setPixelColor(0, 16, 0, 0); strip.show();} //LED RED for next mode.
      }
      break;
  }
  //Keyboard.releaseAll();
}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  Keyboard.begin();
  Consumer.begin();

}

// the loop function runs over and over again forever
void loop() {

  int KeyNum = -1;
  if(!digitalRead(0)) KeyNum =0;
  if(!digitalRead(1)) KeyNum =1;
  if(!digitalRead(2)) KeyNum =2;
  if(!digitalRead(3)) KeyNum =3;
  
  if(KeyNum >= 0 && KeyNum <= 3)
  {
    int start = millis();
    int stopi = millis();
    //int longpush = 0;

    delay(50);
    while (digitalRead(KeyNum) == 0 && ((stopi - start) <= 175)) stopi = millis(); //attente 175ms longpush

    if ((stopi - start) <= 175)
    {   //Key pressed 
        start = millis();
        stopi = millis();
        delay(50);
        while (digitalRead(KeyNum) == 1 && ((stopi - start) <= 175)) stopi = millis(); //attente 175ms detection seconde touche pressé.
        if((stopi - start)<=175)
        { 
          start = millis();
          stopi = millis();
          delay(50);
          while (digitalRead(KeyNum) == 1 && ((stopi - start) <= 175)) stopi = millis(); //attente 175ms detection seconde touche pressé.
          if((stopi - start)<=175)
          {
            Keypress(KeyNum,TRIPLETAP);
          }
          
          else
          {
            //Double TAP
            Keypress(KeyNum,DOUBLETAP);
          }

        }
        else
        { //SIMPLE TAP
          Keypress(KeyNum,TAP);
        }
        delay(50);
    }
    else
    {
      while (digitalRead(KeyNum) == 0)
      {
        Keypress(KeyNum,LONG);
        delay(150);
      }
    }
    Keyboard.releaseAll();
  }
}
