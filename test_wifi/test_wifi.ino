#include "motoWiFiEsp.h"

#include <SoftwareSerial.h>

SoftwareSerial esp8266_Serial(13,12);

WiFiEspClient esp_client;

int connect_status = WL_IDLE_STATUS;

void setup()
{
  esp8266_Serial.begin(9600);

    esp8266_Serial.listen();
  WiFi.init(&esp8266_Serial); 
  if(WiFi.status()==WL_NO_SHIELD) {
   Serial.println(F("Esp8266 module no present"));
   while(true);
   }
  while(connect_status != WL_CONNECTED) {
   Serial.println(F("Connect to router..."));
   connect_status = WiFi.begin("","");//第一個字串輸入wifi的SSID,第二個字串輸入wifi的密碼
  }

}


void loop()
{

}
