//on_error.cpp

#include <Arduino.h>
#include <SD.h>
#include "on_error.h"

void on_error(String error_msg, bool print_msg_via_serial, 
              const char *log_file, File &file,
              int blink_pin) 
//* Stop the program with a endless while-loop.
//* If `log_file` is not "", append `error_msg` to the file (SD must have been started,
//  and `file` will be closed first).
//* If `blink_pin` is >= 0, continuously alternate the pin between HIGH and LOW.
{
  if(print_msg_via_serial)
    Serial.println(error_msg);
   
  if(strlen(log_file) > 0)
  {
    file.close();  
    file = SD.open(log_file, FILE_WRITE);
    file.println(error_msg);
    file.close();
  }
    
  while(true) 
  {
    if(blink_pin > 0)
    {
      #define DELAY_MS 250
      digitalWrite(blink_pin, HIGH);
      delay(DELAY_MS);
      digitalWrite(blink_pin, LOW);
      delay(DELAY_MS);
    }
    else
      delay(100);
  }
}
