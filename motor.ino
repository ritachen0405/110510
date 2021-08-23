#include <SoftwareSerial.h>
//----------below 20161209
#define WaterHigh 50 //mm定義水缸允許的最高水位
#define WaterLow 80 //mm定義該補水的水位低點
#define WaterMidUp 60 //mm定義補水該停止的水位點
#define WaterDeep 120 //mm定義換水時抽水該停止的水面深度
#define WaterDirty 480 //定義該換水的濁度
#define WNTUpin A1 //定義讀取濁度感測的接腳
int buf[5]; //儲存[0]:深度mm,[1]:濁度
//---------above 20161209
//---------echo 超音波感測器設定
const int trig = 5;
const int echo = 6;
const int inter_time = 1000;
const int ON = 1;
const int OFF = 0;
int time = 0;
int In1 =3; //In1 進水的繼電器跟Arduino接的腳位為3
int Out1 =7; //Out1抽水的 繼電器跟Arduino接的腳位為7
//---------below藍芽使用相關設定
const int rxpin =10 ; //接收用的腳位,HC-05只能用PWM腳接收!
const int txpin =11 ; //傳送用的腳位,HC-05只能用PWM腳傳送!
SoftwareSerial bluetooth(rxpin, txpin) ; //給定接腳的新
串列埠, bluetooth(接藍芽RX, 接藍芽TX)
//---------above藍芽使用相關設定
String str;
void setup() {
 Serial.begin(9600) ;
 pinMode (trig, OUTPUT); //改設超音波trig成輸出
 pinMode (echo, INPUT); //改設超音波ceho成輸入
 pinMode(WNTUpin,INPUT); //設置腳位的模式為輸入
 pinMode(In1,OUTPUT); //設置腳位的模式為輸出
 pinMode(Out1,OUTPUT); //設置腳位的模式為輸出 
50
 bluetooth.begin(9600) ; //初始化藍芽通信埠
 Serial.println("Serial ready") ;
 delay(1000) ;
}
void loop() {
 getDeep(); //讀取超音波
 if(buf[0]>WaterLow){ //低於補水點，呼叫補水
 WaterTooLow();
 }
 getDeep(); //讀取超音波
 if(buf[0]<WaterHigh){ //高於最高水位，呼叫抽水
 WaterTooHigh();
 }
 getDirty(); //讀取濁度計
 if(buf[1]<WaterDirty){ //水太濁，呼叫換水
 WaterTooDirty();
 }
 delay(2000);
}
void getDirty(){ //讀取濁度計存到buf[1]
 delay(2000); //等待20ms
 buf[1]=analogRead(WNTUpin); //讀入濁度
 str=String(buf[1]); //將dat[1]轉成str
 Serial.println("Dirty=" + str);
 Data_send(); //送出藍芽讀到數據
 delay(3000) ;
}
void getDeep(){ //讀取水面深度超音波存到buf[0]
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
51
 buf[0]=(int) (fdistance * 10); //讀入超音波
 str=String(buf[0]); //將dat[0]轉成str
 Serial.println("Deep=" + str);
 Data_send(); //送出藍芽讀到數據
 delay(3000) ;
}
void setINwater(int flag){ //開OR關進水馬達
 if (flag==1){
 Serial.println("IN water ON");
 digitalWrite(In1,HIGH); //通電
 }
 else{
 Serial.println("IN water OFF");
 digitalWrite(In1,LOW); //不通電
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
 digitalWrite(Out1,LOW); //不通電
 }
 }
void WaterTooLow(){ //處理水位太低
 Serial.println("Enter water too Low");
 while(buf[0]>WaterMidUp){ //補不到中上水位就繼續補
setOUTwater(OFF); //關出水馬達
setINwater(ON); //開進水馬達
getDeep(); //讀取水位計
}
//補水到達中上水位，進出水馬達通通關掉
setOUTwater(OFF); //關出水馬達
setINwater(OFF); //關進水馬達 
52
}
void WaterTooHigh(){ //處理水位太高
 Serial.println("Enter water too High");
 while(buf[0]<WaterMidUp){ //水位仍高於中上水位就繼續抽
setINwater(OFF); //關進水馬達
setOUTwater(ON); //開出水馬達
getDeep(); //讀取超音波
}
//抽水到達中上水位，進出水馬達通通關掉
setOUTwater(OFF); //關出水馬達
setINwater(OFF); //關進水馬達
}
void WaterTooDirty(){ //處理水太濁
 Serial.println("Enter water too Dirty");
 do{
setINwater(OFF); //關進水馬達
setOUTwater(ON); //開出水馬達
getDeep();
 }while(buf[0]<WaterDeep); //當水面離缸頂不夠深，繼續抽水
//抽水到達最低水面，進出水馬達通通關掉
setOUTwater(OFF); //關出水馬達
setINwater(OFF); //關進水馬達
}
void Data_send() {
 //送出水位數據
 str=String(buf[0]); //將dat[0]轉成str
 Serial.print("Water(mm) = "); //序列埠顯示，監控用
 Serial.println(str); //序列埠顯示，監控用
 str="W#"+str+"\n" ; //組成超音波命令
 Serial.println(str);
 BT_send(str) ; //送出超音波命令
 //送出濁度數據
 str=String(buf[1]); //將dat[1]轉成str
 Serial.print("Turbidity(NTU) = "); //序列埠顯示，監控用
 Serial.println(str); //序列埠顯示，監控用
 str="T#"+str+"\n" ; //組成濁度命令
 Serial.println(str);
 BT_send(str) ; //送出濁度命令 
53
}
void BT_send(String tmp) {
char buffer[30] ;
 tmp.toCharArray(buffer,30) ;
 bluetooth.write(buffer); //將資料輸出到藍芽
}