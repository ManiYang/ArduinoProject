#include "button_state_machine.h"

#include <Arduino.h>

extern int sampling_period; //(ms)
extern byte button_pin;

byte button_state_machine()
//Returns button state.
{
  static byte button_state = 1;
  static int count = 0;
  
  bool pressed = digitalRead(button_pin) == LOW;
  switch(button_state)
  {
    case 0:
      count++;
      if(count*sampling_period > 900) //enter state 1 after being in state 0 for 500 ms
        button_state = 1;
      break;
      
    case 1:
      if(pressed)
        button_state = 2;
      break;
      
    case 2:
      if(pressed)
      {
        button_state = 10;
        count = 0;
      }
      else
        button_state = 1;
      break;
      
    case 10:
      count++;
      if(count*sampling_period > 900) //enter state 11 after being in state 10 for 500 ms
        button_state = 11;
      break;
      
    case 11:
      if(pressed)
        button_state = 12;
      break;
      
    case 12:
      if(pressed)
      {
        button_state = 0;
        count = 0;
      }
      else
        button_state = 11;
      break;
      
    default:
      break;
  }
  
  return button_state;
}
