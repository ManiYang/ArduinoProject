#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#include "SD_card.h"
#include "I2C_my_api.h"
#include "MPU6050.h"
#include "on_error.h"
#include "button_state_machine.h"


/********* settings ***********/
#define DEBUG_VIA_SERIAL 1

const bool to_write_file = true;
const String output_filename_head = "test";
const String output_filename_extension = "dat";

int sampling_period = 100; //(ms)

#define LED_PIN 9   // LOW --> ON
byte button_pin = 8;  //pressed --> LOW

const char *error_log_file = "error_log.txt";
/********* end of settings *********/


void print_via_serial(const String &msg)
{
  #if DEBUG_VIA_SERIAL
    Serial.print(msg);
  #endif
}

File file;
long t_next;

void setup()
{
  #if DEBUG_VIA_SERIAL
    Serial.begin(9600);
    while (!Serial) {}
  #endif

  //
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
   
  pinMode(button_pin, INPUT_PULLUP);
  
  //
  if(!init_SD_card())
    on_error("Could not initialize SD card.", DEBUG_VIA_SERIAL, "", &file, LED_PIN);
  
  if(!remove_file_if_exist(error_log_file))
    on_error(String("Could not remove ")+error_log_file, DEBUG_VIA_SERIAL, "", &file, LED_PIN);
  
  //
  Wire.begin(); //I2C
  if(!init_MPU6050(false))
    on_error("Could not initialize MPU6050.", DEBUG_VIA_SERIAL, error_log_file, &file, LED_PIN);

  //
  print_via_serial("Setup done.\n");
  t_next = millis() + sampling_period;
}

//////////////////////////////////////////////////////////////////////

long t;
bool to_measure = false;
char data_accel[6];
char data_gyro[6];

void loop()
{
  // wait until millis() >= t_next
  long dt = millis() - t_next - 2;
  if(dt > 0)
    delay(dt);
  
  while(millis() < t_next) {}
  
  //
  byte action = button_state_machine();
  if(action == 1) //start measurement
  {
    to_measure = true;
    digitalWrite(LED_PIN, LOW);
    
    // open output file
    if(!open_new_file_with_number_for_writing(&file, output_filename_head, output_filename_extension))
    {
      on_error("Could not open new output file for writing.", DEBUG_VIA_SERIAL, 
               error_log_file, &file, LED_PIN);
    }
    print_via_serial(String("Opened file ")+file.name()+"\n");
  }
  else if(action == 0) //end measurement
  {
    to_measure = false;
    digitalWrite(LED_PIN, HIGH);
    
    // close output file
    file.close();
    print_via_serial("File closed.\n");
  }
  else 
  {
    if(to_measure)
    {
      // perform measurement
      t = millis();
            
      if(!I2C_read_bytes(MPU6050_ADDR, 0x38, 6, data_accel))
      {
        on_error("Error in reading data from MPU6050.", DEBUG_VIA_SERIAL, 
                  error_log_file, &file, LED_PIN);
      }
      
      if(!I2C_read_bytes(MPU6050_ADDR, 0x43, 6, data_gyro))
      {
        on_error("Error in reading data from MPU6050.", DEBUG_VIA_SERIAL, 
                  error_log_file, &file, LED_PIN);
      }
        
      // write to output file  
      short tt = short(t & 0xffff);
      file.write(char(tt >> 8));
      file.write(char(tt & 0xff));

      file.write(data_accel, 6);
      file.write(data_gyro, 6);
      
      file.flush();
    }
  }
  
  //
  t_next += sampling_period;
  while(t_next < millis())
    t_next += sampling_period;
}
