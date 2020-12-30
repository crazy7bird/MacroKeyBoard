#include "HID-Project/HID-Project.h"
#include "ledDriver.h"
#include "secret.h"


enum PRESSTYPE
{
  TAP,
  DOUBLETAP,
  LONG
};

//Prototype passWord actions 

void pressedPwsKey(int Key, int pressType)
{
    if(Key==0)
    {
      if(pressType == TAP)
      {
        LedState=BLUE;
        Keyboard.press(KEY_ENTER);
        delay(75);
        Keyboard.releaseAll();
        delay(75);
        Keyboard.print(LOGIN);
        delay(75);
        Keyboard.press(KEY_ENTER);
        delay(75);
        Keyboard.releaseAll();
      }
    }
    if(Key==1)
    {
        if(pressType == TAP)LedState=GREEN;
    }
    if(Key==2)
    {
      if(pressType == TAP)LedState=GAMING;
    }

    if(Key == 3)
    {
      if(pressType == TAP)LedState=WHITE;
    }
}

void passWord()
{
  long Time = millis();
  LedState = RED;
  while((millis()- Time)< 5000)
  {
    ledDriver();
    poolingKeys(&pressedPwsKey);
  }
}

//Actions after a Key pressed.
void pressedKey(int Key, int pressType)
{
    if(Key==0)
    {
      if(pressType == TAP)Consumer.write(MEDIA_NEXT);
      if(pressType == DOUBLETAP){Consumer.write(MEDIA_NEXT);Consumer.write(MEDIA_NEXT);}
      if(pressType == LONG)Consumer.write(MEDIA_VOLUME_UP);
    }
    if(Key==1)
    {
      if(pressType == TAP)Consumer.write(MEDIA_PREV);
      if(pressType == DOUBLETAP){Consumer.write(MEDIA_PREV);Consumer.write(MEDIA_PREV);}
      if(pressType == LONG)Consumer.write(MEDIA_VOLUME_DOWN);
    }
    if(Key==2)
    {
      if(pressType == TAP)Consumer.write(MEDIA_PLAY_PAUSE);
      if(pressType == DOUBLETAP){Keyboard.press(KEY_LEFT_SHIFT);Keyboard.press(KEY_LEFT_CTRL);Keyboard.print("'");} // ' for m mute in discord  
    }

    if(Key == 3)
    {
      if(pressType == DOUBLETAP){Keyboard.press(KEY_LEFT_WINDOWS);Keyboard.print("o");} //o for l windows + l
      if(pressType == LONG)passWord();
    }
}

// POOLING Keyboard pressing.
void poolingKeys(void (*callback)(int,int))
{
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

    delay(75);
    while (digitalRead(KeyNum) == 0 && ((stopi - start) <= 200)) stopi = millis(); //attente 175ms longpush

    if ((stopi - start) <= 200)
    {   //Key pressed 
        start = millis();
        stopi = millis();
        delay(75);
        while (digitalRead(KeyNum) == 1 && ((stopi - start) <= 200)) stopi = millis(); //attente 175ms detection seconde touche pressé.
        if((stopi - start)<=200)
        { 
          //Double TAP
          (*callback)(KeyNum,DOUBLETAP);
        }
        else
        { //SIMPLE TAP
          (*callback)(KeyNum,TAP);
        }
        delay(200);
    }
    else
    {
      while (digitalRead(KeyNum) == 0)
      {
        (*callback)(KeyNum,LONG);
        delay(125);
      }
    }
    Keyboard.releaseAll();
  }
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
  //Init Leds 
  LedState = GAMING;
  //strip.setPixelColor(0, 16, 0, 0); strip.show();
}

// the loop function runs over and over again forever
void loop() 
{
  ledDriver();
  poolingKeys(&pressedKey);
}
