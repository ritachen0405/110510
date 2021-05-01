#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS  A4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
void setup() {
  Serial.begin(9600); //Baud rate: 9600
  Serial.println("Temperature Sensor");
  sensors.begin();
}
void loop() {
  sensors.requestTemperatures();
  Serial.println(sensors.getTempCByIndex(0));  
  int sensorValue = analogRead(A0);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(voltage); // print out the value you read:
  delay(500);
}
