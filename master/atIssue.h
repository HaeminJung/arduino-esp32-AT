/*
 atIssue.h - Issue AT commands using functions
 Copyright (c) 2019 Fill In. All right reserved.

  After storing list of required AT Commands, using function calls
 you can issue AT commands through UART.
 The basic idea is to store all the data in the RX buffer to a String array,
 then checking for "OK" at the end off the array.
 If an error occurs it can be detected through error_check by setting appropriate
 error code that will be issued by the modem.
  To access the return string, refer to ATResults_int and ATResults_char.
 Char for ASCII characters, int for ASCII codes in integers.

  The modem used in developement was BG96(LTE Cat.M1 module by Queltec). Please
 adjust the error code to fit your purposes.


 sendCommand(HardwareSerial modem, String send_String) - Issue specified command to specified modem.
 readSerial(HardwareSerial modem, String sendString, int count) - After issuing command to modem function stores the return string to an array
 error_check(int count) - Allows to check for Errors.
*/
#include<Arduino.h>

#ifndef atIssue_h
#define atIssue_h

#define ARRAYSIZE 30
#define OK "797513"
#define ERROR "4367776932698282798258"

class AtIssue
{
public:
  AtIssue();

  bool sendCommand(HardwareSerial modem, String send_String);
  int readSerial(HardwareSerial modem, String sendString, int count);
  bool error_check(int count);

  String ATResults_int[ARRAYSIZE] = {};
  String ATResults_char[ARRAYSIZE] = {};

private:
 int8_t count;
 int8_t i;
 int8_t b;
 String at_return_char;
 String at_return_int;
 char a;n
};

#endif
