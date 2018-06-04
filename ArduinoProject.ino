#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "SD_card.h"
#include "MPU6050.h"
#include "on_error.h"


/********* settings ***********/
#define DEBUG_VIA_SERIAL 1

const bool to_write_file = true;

#define LED_PIN 9  //LOW --> ON

const String output_filename_head = "test";
const String output_filename_extension = "dat";

const char *error_log_file = "error_log.txt";
/********* end of settings *********/


File file;

void setup() {
  #if DEBUG_VIA_SERIAL
    Serial.begin(9600);
    while(!Serial) {}
  #endif
  
  //
  if(LED_PIN >= 0)
  {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
  }
    
  //
  if(!init_SD_card())
    on_error("Failed to initialize SD card.", DEBUG_VIA_SERIAL, "", &file, LED_PIN);
  
  if(!remove_file_if_exist(error_log_file))
    on_error("Failed to initialize SD card.", DEBUG_VIA_SERIAL, "", &file, LED_PIN);
  
  //
  Wire.begin(); // I2C
  
  #if DEBUG_VIA_SERIAL
    Serial.print("Initializing MPU6050... ");
  #endif
  if(!init_MPU6050(false))
    on_error("Failed to initialize MPU6050.", DEBUG_VIA_SERIAL, 
             error_log_file, &file, LED_PIN);
  #if DEBUG_VIA_SERIAL
    Serial.println("done");
  #endif
  
  //
  if(to_write_file)
  {
    if(!open_new_file_with_number_for_writing(&file, output_filename_head, 
                                              output_filename_extension))
    {
      on_error("Failed to open output file for writing.", DEBUG_VIA_SERIAL, 
               error_log_file, &file, LED_PIN);
    }
    #if DEBUG_VIA_SERIAL
      Serial.println(String("Opened file \"")+file.name()+"\".");
    #endif
  }
  
  //
  #if DEBUG_VIA_SERIAL
    Serial.println("Setup done.");
  #endif
  if(LED_PIN >= 0)
    digitalWrite(LED_PIN, LOW); //turn on LED
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

void loop() {
    
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
        
    ///
    count++;  */
    delay(100);
}

// how to make sure the sampling rate?  timer?

// error signal? LED?

// when to close the file?  button?

