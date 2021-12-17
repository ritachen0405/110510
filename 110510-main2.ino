#include <SoftwareSerial.h>
#include "motoWiFiEsp.h"
#include <DallasTemperature.h>
#include <Servo.h>
#include "Timer.h"
//----------
#define WaterHigh 50 //mm定義水缸允許的最高水位
#define WaterLow 80 //mm定義該補水的水位低點
#define WaterMidUp 60 //mm定義補水該停止的水位點
#define WaterDeep 120 //mm定義換水時抽水該停止的水面深度
#define WaterDirty 480 //定義該換水的濁度
#define samplingInterval 20
#define printInterval 800 
#define samplingInterval 20
#define printInterval 800
Servo myservo; 
Timer feedingtime;
SoftwareSerial esp8266_Serial(13,12);
WiFiEspClient esp_client;
int connect_status = WL_IDLE_STATUS;
int buf[5]; //儲存[0]:深度mm,[1]:濁度,[2]:溫度
//---------echo 超音波感測器設定
const int trig = 5;
const int echo = 6;
const int inter_time = 1000;
const int ON = 1;
const int OFF = 0;
int feedingsetting = 0;
int time = 0;
int In1 =3; //In1 進水的繼電器跟Arduino接的腳位為3
int Out1 =7; //Out1抽水的 繼電器跟Arduino接的腳位為7
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
     pinMode (trig, OUTPUT); //改設超音波trig成輸出
     pinMode (echo, INPUT); //改設超音波ceho成輸入
     pinMode(In1,OUTPUT); //設置腳位的模式為輸出
     pinMode(Out1,OUTPUT); //設置腳位的模式為輸出 
     myservo.attach(10);//設定伺服馬達腳位
     feedingtime.every(feedingsetting,feeding);
     Serial.println("Serial ready") ;
     delay(1000) ;
}
void loop() {
     getDeep(); //讀取超音波
     if(buf[0]>WaterLow){ //低於補水點，呼叫補水
      WaterTooLow();
     }
     getDeep(); //讀取超音波
     if(buf[0]<WaterHigh){ //高於最高水位，呼叫抽水
      WaterTooHigh();
     }
     getDirty();//取得濁度
     if(buf[1]<WaterDirty){ //水太濁，呼叫換水
      WaterTooDirty();
     }
     setting();//取得自動換水及自動餵食的設定
     feedingtime.update();
}
void setting(){
    String s = thingspeakReadData("PVMXR6L19O1UAGL2","1582446",2, 0);
    feedingsetting = atoi(s.c_str());
}
String clientDataReceive( ) {
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
     String s = thingspeakReadData("ZNCOHBASW59GMTA4","1377526",2, 0);
     buf[1]= atoi(s.c_str());
}
void feeding(){
     myservo.write(0);  
    delay(1000);
    myservo.write(140); 
}
void getDeep(){ //讀取水面深度超音波存到buf[0]
     delay(2000); //等待20ms
     float duration;
     int distance;
     float fdistance;
     digitalWrite(trig, HIGH);
     delayMicroseconds(1000);
     digitalWrite(trig, LOW);
     duration = pulseIn (echo, HIGH);
     //distance = (duration/2)/29;
     fdistance = (duration/2) * 0.034; 
     buf[0]=(int) (fdistance * 10); //讀入超音波
     str=String(buf[0]); //將dat[0]轉成str
     Serial.println("Deep=" + str);
     //將資料傳至thingspeak
     esp8266_Serial.listen();
      if (esp_client.connect("api.thingspeak.com", 80)) {
         String  things_request = "GET /update?api_key=VDKLCDOS06UM85WK&field4="+ String(buf[0])+"\r\n\r\n";
         esp_client.print(things_request);
         esp_client.stop();
       }
     //---------
     delay(3000) ;
}
void setINwater(int flag){ //開OR關進水馬達
     if (flag==1){
         Serial.println("IN water ON");
         digitalWrite(In1,HIGH); //通電
     }
     else{
         Serial.println("IN water OFF");
         digitalWrite(In1,LOW); //不通電
     }
 }
void setOUTwater(int flag){ //開OR關出水馬達
     if (flag==1)
     {
        Serial.println("OUT water ON");
        digitalWrite(Out1,HIGH); //通電
     }
     else
     {
        Serial.println("OUT water OFF");
        digitalWrite(Out1,LOW); //不通電
     }
 }
void WaterTooLow(){ //處理水位太低
    Serial.println("Enter water too Low");
    while(buf[0]>WaterMidUp){ //補不到中上水位就繼續補
        setOUTwater(OFF); //關出水馬達
        setINwater(ON); //開進水馬達
        getDeep(); //讀取水位計
    }
    //補水到達中上水位，進出水馬達通通關掉
    setOUTwater(OFF); //關出水馬達
    setINwater(OFF); //關進水馬達
}
void WaterTooHigh(){ //處理水位太高
    Serial.println("Enter water too High");
    while(buf[0]<WaterMidUp){ //水位仍高於中上水位就繼續抽
        setINwater(OFF); //關進水馬達
        setOUTwater(ON); //開出水馬達
        getDeep(); //讀取超音波
    }
    //抽水到達中上水位，進出水馬達通通關掉
    setOUTwater(OFF); //關出水馬達
    setINwater(OFF); //關進水馬達
}
void WaterTooDirty(){ //處理水太濁
     Serial.println("Enter water too Dirty");
     do{
        setINwater(OFF); //關進水馬達
        setOUTwater(ON); //開出水馬達
        getDeep();
     }while(buf[0]<WaterDeep); //當水面離缸頂不夠深，繼續抽水
    //抽水到達最低水面，進出水馬達通通關掉
    setOUTwater(OFF); //關出水馬達
    setINwater(OFF); //關進水馬達
}
