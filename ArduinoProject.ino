#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "SD_card.h"
#include "I2C_my_api.h"
#include "MPU6050.h"
#include "on_error.h"
//#include "button_state_machine.h"


/********* settings ***********/
#define DEBUG_VIA_SERIAL 0

const bool to_write_file = true;
const String output_filename_head = "test";
const String output_filename_extension = "dat";

int sampling_period = 100; //(ms)

#define LED_PIN -1   // LOW --> ON  (-1: not used)
//byte  = 8;  //pressed --> LOW

#define RXPIN_FROM_BT 8  //connected to TX of BT module
#define TXPIN_TO_BT 9    //connected to RX of BT module 
#define BT_BAUD 9600
#define START_CODE '1'  //sent to Arduino via BT
#define STOP_CODE '0'   //sent to Arduino via BT

const char *error_log_file = "error_log.txt";
/********* end of settings *********/


void print_via_serial(const String &msg)
{
  #if DEBUG_VIA_SERIAL
    Serial.print(msg);
  #endif
}

SoftwareSerial BT(RXPIN_FROM_BT, TXPIN_TO_BT);
File file;
long t_next;

void setup()
{
  #if DEBUG_VIA_SERIAL
    Serial.begin(9600);
    while (!Serial) {}
  #endif

  // LED & button
  //pinMode(LED_PIN, OUTPUT);
  //digitalWrite(LED_PIN, HIGH);
   
  //pinMode(button_pin, INPUT_PULLUP);
  
  // BT
  BT.begin(BT_BAUD);
  
  // SD card
  if(!init_SD_card())
    on_error("Could not initialize SD card.", DEBUG_VIA_SERIAL, "", &file, LED_PIN);
  
  if(!remove_file_if_exist(error_log_file))
    on_error(String("Could not remove ")+error_log_file, DEBUG_VIA_SERIAL, "", &file, LED_PIN);
  
  // MPU6050
  Wire.begin(); //I2C
  if(!init_MPU6050(false))
    on_error("Could not initialize MPU6050.", DEBUG_VIA_SERIAL, error_log_file, &file, LED_PIN);

  //
  print_via_serial("Setup done.\n");
  t_next = millis() + sampling_period;
}

//////////////////////////////////////////////////////////////////////

long t;
bool measuring = false;
char data_accel[6];
char data_gyro[6];
char action = '\0';

void loop()
{
  // wait until millis() >= t_next
  long dt = millis() - t_next - 2;
  if(dt > 0)
    delay(dt);
  
  while(millis() < t_next) {}
  
  // get action code
  //action = button_state_machine();
  if(BT.available())
    action = BT.read();
  else 
    action = '\0';
  
  //
  if(action == START_CODE) //start measurement
  {
    if(!measuring)
    {
      measuring = true;

      // open output file
      if(!open_new_file_with_number_for_writing(&file, output_filename_head, output_filename_extension))
      {
        on_error("Could not open new output file for writing.", DEBUG_VIA_SERIAL, 
                 error_log_file, &file, LED_PIN);
      }

      //
      print_via_serial(String("Opened file ")+file.name()+"\n");
      BT.println("Starting measurement.");
      BT.println(String("Output file: ")+file.name());
      //digitalWrite(LED_PIN, LOW);
    }
  }
  else if(action == STOP_CODE) //stop measurement
  {
    if(measuring)
    {
      measuring = false;

      // close output file
      file.close();

      //
      print_via_serial("File closed.\n");
      BT.println("Stopped measurement.");
      //digitalWrite(LED_PIN, HIGH);
    }
  }
  else //continue idling or measuring
  {
    if(measuring)
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
