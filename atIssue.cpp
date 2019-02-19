/*
AtIssue.cpp - Issue AT commands using functions
Copyright (c) 2019 Fill In. All right reserved.

*/

#include "atIssue.h"

AtIssue::AtIssue(){
 }

bool AtIssue::sendCommand(HardwareSerial modem, String send_String){
  int count = 0;  //initialize address holder

  for(int i=1;i<=5;i++){
    count = readSerial(modem, send_String, count);

    if(ATResults_int[count]==OK){
      return true;
      break;
    }

    if(error_check(count)){
      break;
    }

    Serial.println("OK not returned. Retrying after 500ms");
    Serial.print("Tries out of ");
    Serial.print(i);
    Serial.println("/5");
    delay(500);

    if(i=5){
      Serial.println("OK not returned. Exiting"); //if AT fails here, there need an intrupt to reboot the modem
    }
  }

  return false;
}

int AtIssue::readSerial(HardwareSerial modem, String sendString, int count){
  String at_return_char = "";
  String at_return_int = "";

  modem.println(sendString);
  Serial.print("Sending: ");
  Serial.println(sendString);

  delay(300); // delay for AT return to arrive

  while (modem.available() != 0){ //check for line ending
   char a = modem.read();
   int b = (int) a;

//If the UART spits out shit long craziness, check if the modem is on
   if(sizeof(b)>100){
     Serial.print("Error: Please check if BG96 is turned on properly.");
     break;
   }

   if(b==10){
    ATResults_char[count] = at_return_char;
    ATResults_int[count] = at_return_int;
    Serial.println(ATResults_char[count]);
    //Serial.println(ATResults_int[count]);

    count++;

    at_return_char = "";
    at_return_int = "";
   } else{
    at_return_char += a;
    at_return_int += b;
   }



  }
  return count-1;
}

bool AtIssue::error_check(int count){
  String errorCode = ATResults_int[count];
  if(errorCode.substring(0,22)==ERROR){
    Serial.println("Error Occured. Refer to the Error Code");
    return true;
  }
  return false;
}
