//on_error.cpp

#include <Arduino.h>
#include "on_error.h"

void on_error(String error_msg, bool print_msg_via_serial) 
{
  if(print_msg_via_serial)
    Serial.println("Error: "+error_msg);
   
  while(true) 
  {
    delay(100);
  }
}