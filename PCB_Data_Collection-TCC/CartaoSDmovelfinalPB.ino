#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "ClosedCube_HDC1080.h"
#include <Arduino.h>
#include <SensirionI2cScd30.h>

ClosedCube_HDC1080 hdc1080;
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
SensirionI2cScd30 sensor;

const int chipSelect = 10;
File myFile;
unsigned long int millisAtual;
char tempo[8];

void setup() {
  pinMode(2, OUTPUT);
  sensors.begin();
  hdc1080.begin(0x40);
  Wire.begin();
  sensor.begin(Wire, SCD30_I2C_ADDR_61);
  sensor.startPeriodicMeasurement(0);  
  sensor.softReset();
  if (!SD.begin(chipSelect)) {
    digitalWrite(2, HIGH); // LED aceso se não iniciou SD
    while (true);
  }
}
void loop() {
  myFile = SD.open("temp.txt", FILE_WRITE);
  sensors.requestTemperatures();
  float co2Concentration = 0.0;
  float temperature = 0.0;
  float humidity = 0.0;
  sensor.blockingReadMeasurementData(co2Concentration, temperature, humidity); 
  millisAtual=millis();
  unsigned long int segundosTotais = millisAtual / 1000;
  unsigned long int horas = segundosTotais / 3600;
  unsigned long int minutos = (segundosTotais % 3600) / 60;
  unsigned long int segundos = segundosTotais % 60;
  if (myFile) {
    digitalWrite(2, LOW); // LED apagado se arquivo abriu
    sprintf(tempo, "%02lu%02lu%02lu;", horas, minutos, segundos);
    myFile.print(tempo);
    myFile.print(sensors.getTempCByIndex(0));
    myFile.print(";");
    myFile.print(hdc1080.readTemperature());
    myFile.print(";");
    myFile.print(temperature);
    myFile.print(";");
    myFile.print(hdc1080.readHumidity());
    myFile.print(";");
    myFile.print(humidity);
    myFile.print(";");
    myFile.print(co2Concentration);
    myFile.println();
    myFile.close();
    delay(59500);
  } else {
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(200);
  }
}