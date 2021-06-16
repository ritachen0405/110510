#include "motoWiFiEsp.h"

#include <SoftwareSerial.h>

SoftwareSerial esp8266_Serial(12,13);

WiFiEspClient esp_client;

int connect_status = WL_IDLE_STATUS;

float Temperature_LM35T_A4(int tempUnit) {
int readtempValue = analogRead(A4);
float temperature = (readtempValue * 0.49);
if(tempUnit == 1)
   return temperature;
 else
   return ((temperature * 1.8) + 32);
}

void setup()
{
  esp8266_Serial.begin(9600);
  Serial.begin(9600);//設定Baud rate: 9600
  pinMode(A4, INPUT);
    esp8266_Serial.listen();
  WiFi.init(&esp8266_Serial); 
  if(WiFi.status()==WL_NO_SHIELD) {
   Serial.println(F("Esp8266 module no present"));
   while(true);
   }
  while(connect_status != WL_CONNECTED) {
   Serial.println(F("Connect to router..."));
   connect_status = WiFi.begin("kunlung","a123056056");
  }

}


void loop()
{
    delay(7200000);//延遲2hr
    esp8266_Serial.listen();//wifi
    int sensorValue = analogRead(A0);//濁度感測(腳位)
    float voltage = sensorValue * (5.0 / 1024.0);//濁度感測(轉換數值)
    if (esp_client.connect("api.thingspeak.com", 80)) {
       String  things_request_1 = "GET /update?api_key=VDKLCDOS06UM85WK&field1="+ String(Temperature_LM35T_A4(1))+"\r\n\r\n";//溫度
       String  things_request_2 = "GET /update?api_key=VDKLCDOS06UM85WK&field2="+ String(voltage)+"\r\n\r\n";//濁度
         esp_client.print(things_request_1);
         esp_client.print(things_request_2);
         esp_client.stop();
    }

}
