//SD_card.h
/*
For Arduino Uno, SD card module should be connected to the SPI bus as follows:
  MOSI - pin 11
  MISO - pin 12
  CLK - pin 13
  CS - pin 10
  
Remember to initialize first (call init_SD_card()).
*/

#ifndef SD_CARD_H
#define SD_CARD_H

#include <SD.h>

inline bool init_SD_card() 
{
  return SD.begin();
}

inline bool open_file_for_appending(File *file, const char *filename);
//File will be created if it does not already exist.

inline bool open_new_file_for_writing(File *file, const char *filename);
//Returns false if the file already exists or cannot be created for writing.

inline bool open_new_file_with_number_for_writing(File *file, 
                                                  const String &filename_head, const String &filename_extension);
//Create a new file named <filename_head>_<x>.<filename_extension>" for writing, where x is a number.

#endif
