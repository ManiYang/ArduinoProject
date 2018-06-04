#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#include "SD_card.h"
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

void setup() {
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
  if(!open_new_file_with_number_for_writing(&file, output_filename_head, output_filename_extension))
  {
    on_error("Could not open new output file for writing.", DEBUG_VIA_SERIAL, 
             error_log_file, &file, LED_PIN);
  }
  print_via_serial(String("Opened file ")+file.name()+"\n");

  //
  print_via_serial("Setup done.\n");
  t_next = millis() + sampling_period;
}

///////////////////////////////////////////////////////////////////////////////////
/*
  char dataAcce[6];
  char dataGyro[6];
  int count = 0;
  long t0, dt;

  void readMPU6050(const int reg, const int byteCount, char *dataBuffer) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    if(Wire.endTransmission() != 0) {
        #if DEBUG_VIA_SERIAL
            error_msg = "I2C transmission error!";
        #endif
        on_error("");
    }
    if(Wire.requestFrom(MPU6050_ADDR, byteCount) != byteCount) {
        #if DEBUG_VIA_SERIAL
            error_msg = "I2C request error!";
        #endif
        on_error("");
    }
    for(int i=0; i<byteCount; i++)
        dataBuffer[i] = Wire.read();
  }
*/
////////

long t;

void loop()
{
  // wait until millis() >= t_next
  long dt = millis() - t_next - 2;
  if(dt > 0)
    delay(dt);
  
  while(millis() < t_next) {}
  
  //
  byte s = button_state_machine();
  bool to_measure = (s == 10 || s == 11 || s == 12);
  
  digitalWrite(LED_PIN, to_measure ? LOW : HIGH);
  
  t = millis();
  
  
  //
  t_next += sampling_period;
  while(t_next < millis())
    t_next += sampling_period;
  

  /*
    if(count >= 100) {
        #if DEBUG_VIA_SERIAL
            Serial.println("done");
        #endif

        #if WRITE_TO_FILE
            file.close();
        #endif

        while(true) {}
    }

    ///
    t0 = millis();

    // read accelerometer data
    readMPU6050(0x3B, 6, dataAcce);

    // read gyroscope data
    readMPU6050(0x43, 6, dataGyro);

    // write to file
    #if WRITE_TO_FILE
        for(int i=0; i<6; i++) {
            file.print(dataAcce[i]);
        }
        for(int i=0; i<6; i++) {
            file.print(dataGyro[i]);
        }
        file.flush();
    #endif

    //
    dt = millis() - t0;

    /// print data
    #if DEBUG_VIA_SERIAL
        Serial.print("dt: ");
        Serial.println(dt);

    #endif
*/
}

// how to make sure the sampling rate?  timer?


// when to close the file?  button?

