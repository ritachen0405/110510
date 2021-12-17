#include <SoftwareSerial.h>
#include "motoWiFiEsp.h"
#include <DallasTemperature.h>
#include <Servo.h>
#include "Timer.h"
//----------
#define WaterDirty 480 //定義該換水的濁度
#define WNTUpin A1 //定義讀取濁度感測的接腳
#define ONE_WIRE_BUS  A4//定義溫度的腳位
#define SensorPin A2            //定義ph值的腳位
#define Offset 0.00            //deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40   #define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40   
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;    
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Servo myservo; 
int buf[5]; //儲存[0]:深度mm,[1]:濁度,[2]:溫度
int feedingsetting = 0;
int time = 0;
SoftwareSerial esp8266_Serial(13,12);
WiFiEspClient esp_client;
int connect_status = WL_IDLE_STATUS;
String str;
int resetTimes=0;
void setup() {
     //wifi設定
     // put your setup code here, to run once:
     setting();
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
          if(resetTimes>=10)break;
          resetTimes++;
      }
     //-------------------------
     Serial.begin(9600) ;
     pinMode(WNTUpin,INPUT); //設置腳位的模式為輸入
     myservo.attach(10);//設定伺服馬達腳位
     sensors.begin();
     Serial.println("Serial ready") ;
     delay(1000) ;
}
void loop() {
     getDirty(); //讀取濁度計
     getTemperature();//取得溫度值
     getPH();
     setting();//取得自動換水及自動餵食的設定
}
void getPH(){
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
//            digitalWrite(LED,digitalRead(LED)^1);
           printTime=millis();
      }
  }
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
void setting(){
    String s = thingspeakReadData("PVMXR6L19O1UAGL2","1582446",1, 0);
    feedingsetting = atoi(s.c_str());
}
String clientDataReceive() {
 uint8_t myData[256]={0};
 int i=0;
 while(1) {
    if (esp_client.available()) {
    char c = esp_client.read();
    myData[i++] = c;
   }else{
       String inData((const char*)myData);
    return inData;
    }
  }
  return "";
}
String thingspeakReadData(String read_key, String channel_id, int field_id, int record_id) {
  uint8_t myData[32]={0};
  int dataIdx=0;
  String  inData="";
  if (esp_client.connect("api.thingspeak.com", 80)) {
     String  things_request = "GET /channels/"+String(channel_id)+"/fields/"+String(field_id)+"/last?api_key="+String(read_key)+"\r\n\r\n";
     esp_client.print(things_request);
     delay(2000);
     inData = clientDataReceive();
     return inData;
  }
}
void getDirty(){ //讀取濁度計存到buf[1]
     delay(2000); //等待20ms
     buf[1]=analogRead(WNTUpin); //讀入濁度
     str=String(buf[1]); //將dat[1]轉成str
     Serial.println("Dirty=" + str);
     //將資料傳至thingspeak
     esp8266_Serial.listen();
      if (esp_client.connect("api.thingspeak.com", 80)) {
         String  things_request = "GET /update?api_key=VDKLCDOS06UM85WK&field2="+ String(buf[1])+"\r\n\r\n";
         esp_client.print(things_request);
         esp_client.stop();
       }
     //---------
     delay(3000);
}
void feeding(){
    myservo.write(0);  
    delay(1000);
    myservo.write(140); 
}
void getTemperature(){
    sensors.requestTemperatures();
    buf[2]=sensors.getTempCByIndex(0);
    str=String(buf[2]);
    Serial.println("溫度："+str);
    esp8266_Serial.listen();
      if (esp_client.connect("api.thingspeak.com", 80)) {
         String  things_request = "GET /update?api_key=VDKLCDOS06UM85WK&field1="+ String(buf[2])+"\r\n\r\n";
         esp_client.print(things_request);
         esp_client.stop();
        }
}
