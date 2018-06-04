// on_error.h
#ifndef ON_ERROR_H
#define ON_ERROR_H

void on_error(String error_msg, bool print_msg_via_serial, 
              const char *log_file, File &file,
              int blink_pin);
//* Stop the program with a endless while-loop.
//* If `log_file` is not "", append `error_msg` to the file (SD must have been started,
//  and `file` will be closed first).
//* If `blink_pin` is >= 0, continuously alternate the pin between HIGH and LOW.

#endif
