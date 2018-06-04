//SD_card.cpp

#include "SD_card.h"

bool open_file_for_appending(File *file, const char *filename) 
//File will be created if it does not already exist.
{
  file->close();
  *file = SD.open(filename, FILE_WRITE); 
  if(*file)
    return true;
  else
    return false;
}

bool open_new_file_for_writing(File *file, const char *filename)
//Returns false if the file already exists or cannot be created for writing.
{
  if(SD.exists(filename))
    return false;
  return open_file_for_appending(file, filename);
}

bool open_new_file_with_number_for_writing(File *file, 
                                           const String &filename_head, const String &filename_extension)
//Create a new file named <filename_head>_<x>.<filename_extension>" for writing, where x is a number.
{
  #define X_MAX 999
  
  bool b = false;
  String fname;
  for(int x=0; x<=(X_MAX); x++)
  {
    fname = filename_head+"_"+String(x)+"."+filename_extension;
    if(!SD.exists(fname))
    {
      b = true;
      break;
    }
  }
  
  if(!b)
    return false;
  else
    return open_file_for_appending(file, fname.c_str());
}

bool remove_file_if_exist(const char *filename)
//Returns true if `filename` does not exist.
{
  if(SD.exists(filename))
    return SD.remove(filename);
  else
    return true;
}
