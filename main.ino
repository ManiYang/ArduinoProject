/*
SD card module should be attached to SPI bus as follows:
  MOSI - pin 11
  MISO - pin 12
  CLK - pin 13
  CS - pin 10
*/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>

/********* settings ***********/
#define DEBUG_VIA_SERIAL 1
#define WRITE_TO_FILE 0
#define FILENAME "test02.dat"
#define MPU6050_ADDR 0x68


//// variables //////////////////////////////////////////////////////////////////
#if DEBUG_VIA_SERIAL
    String error_msg;
#endif

#if WRITE_TO_FILE
    File file;
#endif

/////////////////////////////////////////////////////////////////////////////////
void onError() {
    #if DEBUG_VIA_SERIAL
        Serial.println("Error: "+error_msg);
    #endif

    while(true) {
        //.................
        delay(100);
    }
}

//// initializations ////////////////////////////////////////////////////////////
inline void initSdCard() {
    if(!SD.begin()) {
        #if DEBUG_VIA_SERIAL        
            error_msg = "SD card initialization failed!";
        #endif
        onError();    
    }
}

inline void openFile() {
    #if WRITE_TO_FILE
        file = SD.open(FILENAME, FILE_WRITE); //append to file
        if(!file) {
            #if DEBUG_VIA_SERIAL        
                error_msg = "Failed to open file for writing!";
            #endif
            onError();
        }
    #endif
}

inline void initMPU6050() {
    // exit sleep mode and disable temperature sensor
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x6B);
    Wire.write(0x08);
    //Wire.write(0x00);
    if(Wire.endTransmission() != 0) {
        #if DEBUG_VIA_SERIAL
            error_msg = "I2C transmission error in initMPU6050()!";
        #endif
        onError();
    }
    
    // set sampling rate divider to 7
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x19);
    Wire.write(0x07);
    if(Wire.endTransmission() != 0) {
        #if DEBUG_VIA_SERIAL
            error_msg = "I2C transmission error in initMPU6050()!";
        #endif
        onError();
    }
}

/////////////////////////////////////////////////////////////////////////////////
void setup() {
    #if DEBUG_VIA_SERIAL
        // Open serial communications and wait for port to open:
        Serial.begin(9600);
        while(!Serial) {}
    #endif
    
    initSdCard();
    
    #if WRITE_TO_FILE
        openFile();    
    #endif
    
    Wire.begin(); //I2C init.
    initMPU6050();
        
    #if DEBUG_VIA_SERIAL
        Serial.println("Initialization done.");
    #endif
} //setup()



///////////////////////////////////////////////////////////////////////////////////

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
        onError();
    }
    if(Wire.requestFrom(MPU6050_ADDR, byteCount) != byteCount) {
        #if DEBUG_VIA_SERIAL
            error_msg = "I2C request error!";
        #endif
        onError();
    }
    for(int i=0; i<byteCount; i++) 
        dataBuffer[i] = Wire.read();
}

////////

void loop() {
    
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
        /*
        for(int i=0; i<6; i++) {
            Serial.print(int(dataAcce[i]));
            Serial.print(" ");
        }
        Serial.print("\t\t");
        for(int i=0; i<6; i++) {
            Serial.print(int(dataGyro[i]));
            Serial.print(" ");
        }
        Serial.print("\n");
        */
    #endif
        
    ///
    count++;
    delay(100);
}

// how to make sure the sampling rate?  timer?

// error signal? LED?

// when to close the file?  button?

