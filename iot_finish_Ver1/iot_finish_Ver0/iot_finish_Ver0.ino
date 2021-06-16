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
//----------------------------------------------------------
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;    
//---------------------------------------------------------------PH值
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
    //----------------------------------------------
    static unsigned long samplingTime = millis();
    static unsigned long printTime = millis();
    static float pHValue,voltage;
    if(millis()-samplingTime > samplingInterval)
    {
        pHArray[pHArrayIndex++]=analogRead(SensorPin);
        if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
        voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
        pHValue = 3.5*voltage+Offset;
        samplingTime=millis();
    }
    if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
    {
      Serial.print("Voltage:");
          Serial.print(voltage,2);
          Serial.print("    pH value: ");
          Serial.println(pHValue,2);
          printTime=millis();
    }
    //----------------------------------------------
    esp8266_Serial.listen();//wifi
    int sensorValue = analogRead(A2);//濁度感測(腳位)
    float voltage_j = sensorValue * (5.0 / 1024.0);//濁度感測(轉換數值)
    if (esp_client.connect("api.thingspeak.com", 80)) {
       
       String  things_request_1 = "GET /update?api_key=VDKLCDOS06UM85WK&field1="+ String(Temperature_LM35T_A4(1))+"\r\n\r\n";//溫度
       String  things_request_2 = "GET /update?api_key=VDKLCDOS06UM85WK&field2="+ String(voltage_j)+"\r\n\r\n";//濁度
       String  things_request_3 = "GET /update?api_key=VDKLCDOS06UM85WK&field3="+ String(pHValue)+"\r\n\r\n";//PH值
         esp_client.print(things_request_1);
         esp_client.print(things_request_2);
         esp_client.print(things_request_3);
         esp_client.stop();
    }

}
//--------------------------------------------
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
