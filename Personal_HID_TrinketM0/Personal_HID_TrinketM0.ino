#include "HID-Project/HID-Project.h"
#include "ledDriver.h"
#include "secret.h"

#define TEMPO 75
enum PRESSTYPE
{
  TAP,
  DOUBLETAP,
  LONG
};

//Prototype passWord actions 
int g_PSWDONE ; //Ugly global var 
void pressedPwsKey(int Key, int pressType)
{
    if(Key==0)
    {
      if(pressType == TAP)
      {
        Keyboard.print(START);
        delay(TEMPO);
        Keyboard.press(KEY_ENTER);
        delay(TEMPO);
        Keyboard.releaseAll();
        Keyboard.press(KEY_ENTER);
        delay(TEMPO);
        Keyboard.releaseAll();
      }
    }
    if(Key==1)
    {
      if(pressType == TAP)
      {
        Keyboard.press(KEY_ENTER);
        delay(TEMPO);
        Keyboard.releaseAll();
        delay(TEMPO);
        Keyboard.print(LOGIN1);
        delay(TEMPO);

        Keyboard.press(KEY_LEFT_SHIFT);
        delay(TEMPO);
        Keyboard.print(LOGIN2);
        Keyboard.releaseAll();
        delay(TEMPO);
        Keyboard.print(LOGIN3);
        delay(TEMPO);
        Keyboard.press(KEY_ENTER);
        delay(TEMPO);
        Keyboard.releaseAll();
      }
    }

    if(Key==2)
    {
      if(pressType == TAP){Keyboard.press(KEY_LEFT_WINDOWS);Keyboard.print("o");} //o for l windows + l
    }
    if(Key == 3);
    
    g_PSWDONE=1;
}

void passWord()
{
  long Time = millis();
  char oldLed = LedState;
  LedState = RED;
  g_PSWDONE = 0;
  while(!digitalRead(3))ledDriver();
  while((millis()- Time)< 5000 && g_PSWDONE==0)
  {
    ledDriver();
    poolingKeys(&pressedPwsKey);
  }
  LedState = oldLed;
}

//Actions after a Key pressed.
void pressedKey(int Key, int pressType)
{
    if(Key==0)
    {
      if(pressType == TAP)Consumer.write(MEDIA_NEXT);
      if(pressType == DOUBLETAP){Keyboard.press(KEY_LEFT_WINDOWS); Keyboard.press(KEY_LEFT_CTRL); Keyboard.press(KEY_RIGHT_ARROW);} //Windows next virtual desktop win+ctrl+ ->
      if(pressType == LONG)Consumer.write(MEDIA_VOLUME_UP);
    }
    if(Key==1)
    {
      if(pressType == TAP)Consumer.write(MEDIA_PREV);
      if(pressType == DOUBLETAP){Keyboard.press(KEY_LEFT_WINDOWS); Keyboard.press(KEY_LEFT_CTRL); Keyboard.press(KEY_LEFT_ARROW);}//Windows prev virtual desktop win+ctrl+<-
      if(pressType == LONG)Consumer.write(MEDIA_VOLUME_DOWN);
    }
    if(Key==2)
    {
      if(pressType == TAP)Consumer.write(MEDIA_PLAY_PAUSE);
      if(pressType == DOUBLETAP){Keyboard.press(KEY_LEFT_WINDOWS); Keyboard.press(KEY_LEFT_CTRL); Keyboard.print("i");}//Windows new virtual desktop win+ctrl+D
      if(pressType == LONG){Keyboard.press(KEY_LEFT_SHIFT);Keyboard.press(KEY_LEFT_CTRL);Keyboard.print("'");} // ' for m mute in discord  
    }

    if(Key == 3)
    {
      if(pressType == TAP){if(LedState==GAMING){LedState = WHITE;}else{LedState=GAMING;}} 
      if(pressType == DOUBLETAP){Keyboard.press(KEY_LEFT_WINDOWS); Keyboard.press(KEY_LEFT_CTRL); Keyboard.press(HID_KEYBOARD_F4);}//Windows del virtual desktop win+ctrl+F4
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
