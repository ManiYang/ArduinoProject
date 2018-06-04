#include <SPI.h>
#include <SD.h>
#include <Wire.h>


/********* settings ***********/
#define DEBUG_VIA_SERIAL 1
//const bool write_to_file = true;
//#define FILENAME "test02.dat"

/********* end of settings *********/


#include "on_error.h"
#include "SD_card.h"
#include "MPU6050.h"

File file;

//// initializations ////////////////////////////////////////////////////////////

void setup() {
  #if DEBUG_VIA_SERIAL
    Serial.begin(9600);
    while(!Serial) {}
  #endif
  
  //
  if(!init_SD_card())
  {
    
  }

  //
  Wire.begin(); // I2C
  if(!init_MPU6050(false))
  {
    
  }
  
  
  
    /*
  
    
  if(write_to_file)
    open_file_for_appending(FILENAME);
  
  
  
    
        
    #if DEBUG_VIA_SERIAL
        Serial.println("Initialization done.");
    #endif
    */
} //setup()



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

