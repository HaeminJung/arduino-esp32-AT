# arduino-esp32-AT
Using Arduino IDE and ESP32 to issue AT Commands via function call.

 After storing list of required AT Commands, using function calls you can issue AT commands through UART. The basic idea is to store all the incoming data in the RX buffer to a String array, then checking for "OK" at the end off the array. 
 If an error occurs it can be detected through error_check by setting appropriate error code that will be issued by the modem.
 
 To access the return string, refer to ATResults_int and ATResults_char. (Char for ASCII characters, int for ASCII codes in integers.)

 The modem used in developement was BG96(LTE Cat.M1 module by Queltec). Please
adjust the error code to fit your purposes.
