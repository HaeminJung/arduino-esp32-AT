/*
*  This is an example using BG96 as the modem
*
*/


#include<atIssue.h>

AtIssue at;

HardwareSerial bg96(2);
//////////////////////////////DeviceID Holders
String baudrate;
String manufacturer;
String model;
String ver_firmware;
String imei;
String imsi;
String iccid;

/////////////////////////////GPS Data Holders
String utc;
String latitude;
String longitude;
String altitude;
String gps_parser;

////////////////////////////// String  Array to hold AT COMMANDS
#define ATARRAY 30
String ATInitCommand[ATARRAY] = {"AT", "ATV1", "ATE0", "AT+CMEE=2", "AT+IPR?", "ATI", "AT+GSN", "AT+QURCCFG=\"URCPORT\",\"uart1\"", "AT+CPIN?", "AT+CIMI", "AT+QCCID", "AT+CSQ"};
String ATGNSS[ATARRAY] = {"AT+QGPS=1", "AT+QGPSCFG=\"nmeasrc\",0", "AT+QGPSCFG=\"autogps\",1", "AT+QGPSCFG=\"gnssconfig\",0", "AT+QGPSLOC=2"};

///////////////////////////////////////////////////////////
/*init_BOOT will boot up the device, retrieving required info from the modem.
* The commands are issues in following order.
* AT->ATV1->ATE0->AT+CMEE=2->AT+IPR=?->ATI->AT+GSM->AT+QURCCFG="URCPORT","uart1"
* ->AT+CPIN?->AT+CIMI->AT+QCCID->AT+CSQ
*
* -ATV should be changed to 0 when embedding
* -CMEE will be changed to 1 for error checking
* -QURCCFG needs check.
*/
void init_BOOT(HardwareSerial modem, int baud){
  Serial.println("Begin Conection");
   bool at_check = false;

  Serial.println("initilizing UART");

  while (modem.available()){
    Serial.println("connection failed, restarting connection");
    modem.begin(baud);
  }

  for(int i=0;i<12;i++){
    at_check = at.sendCommand(modem,ATInitCommand[i]);

    if(!at_check){
      Serial.println("AT failed. Reinitializing"); //find a way to reboot....
      break;
    }

    if(i==4){
      baudrate = at.ATResults_char[1];
    } else if(i==5){
      manufacturer = at.ATResults_char[1];
      model = at.ATResults_char[2];
      ver_firmware = at.ATResults_char[3];
    } else if(i==6){
      imei = at.ATResults_char[1];
    } else if(i==9){
      imsi = at.ATResults_char[1];
    } else if(i==10){
      iccid = at.ATResults_char[1];
    }
  }

  Serial.println("BG96 initiated");
}

///////////////////////////////////////////////////////////
/*init_GPS will initialize GPS to required state. The state is as following
* AT+QGPS=1 //turn on gps
* AT+QGPSCFG=\"nmeasrc\",0 //disable nmea
* AT+QGPSCFG=\"autogps\",1 //turn autoGPS ON
* AT+QGPSCFG=\"gnssconfig\",0 //enable only GPS
* AT+QGPSCFG=\"outport\",0 //disable direct nmea
*/
void init_GNSS(HardwareSerial modem){
  Serial.println("Begin GNSS");
  bool at_check = false;

  for(int i=0;i<4;i++){
    at_check = at.sendCommand(modem,ATGNSS[i]);

    if(!at_check){
      break;
    }
    Serial.println("GPS initiated");
  }
}

///////////////////////////////////////////////////////////
/*get_GPS will fill in UTC, Lat, Long, Alt strings.
* It will parse the data from QGPSLOC
*/

void get_GPS(HardwareSerial modem){
  Serial.println("Collecting GPS Data");
  at.sendCommand(modem,ATGNSS[4]);
  gps_parser = at.ATResults_char[1];
  parseGPS();
}

///////////////////////////////////////////////////////////
/*parse_GPS Will need some more quality work...
*/
void parseGPS(){
  utc+=gps_parser.substring(10,18);
  latitude+=gps_parser.substring(19,27);
  longitude+=gps_parser.substring(28,37);
}

////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  bg96.begin(115200);
  Serial.println("Press Button to boot");  //ATcommand Start
}

void loop() {
  if (Serial.available()){

    Serial.println("\n------------\n");
    Serial.println("BG96 initiating");
    Serial.println("\n------------\n");
    init_BOOT(bg96, 115200);
    Serial.println("\n------------\n");
    Serial.println("Logging retrieved data");
    Serial.println(baudrate);
    Serial.println(manufacturer);
    Serial.println(model);
    Serial.println(ver_firmware);
    Serial.println(imei);
    Serial.println(imsi);
    Serial.println(iccid);
    Serial.println("\n------------\n");

    Serial.println("\n------------\n");
    Serial.println("GPS initiatng");
    init_GNSS(bg96);
    Serial.println("\n------------\n");

    Serial.println("\n------------\n");
    Serial.println("retreiving GPS data");
    get_GPS(bg96);

    Serial.println("\n------------\n");
    Serial.println("Logging retrieved data");
    Serial.println(utc);
    Serial.println(latitude);
    Serial.println(longitude);
    Serial.println("\n------------\n");

    Serial.println("Entering Halt");

    delay(50000);
  }
}
