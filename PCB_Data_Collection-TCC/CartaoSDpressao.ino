#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;
const int chipSelect = 10;
File myFile;
unsigned long int millisAtual;
char tempo[8];

void setup() {
  pinMode(9, OUTPUT);
  bmp.begin(0x76);
  Wire.begin();
  
  if (!SD.begin(chipSelect)) {
    digitalWrite(9, HIGH);
    while (true);
  }
}
void loop() {
  myFile = SD.open("press.txt", FILE_WRITE);
  millisAtual=millis();
  unsigned long int segundosTotais = millisAtual / 1000;
  unsigned long int horas = segundosTotais / 3600;
  unsigned long int minutos = (segundosTotais % 3600) / 60;
  unsigned long int segundos = segundosTotais % 60;
  if (myFile) {
    digitalWrite(9, LOW); // LED apagado se arquivo abriu
    sprintf(tempo, "%02lu%02lu%02lu;", horas, minutos, segundos);
    myFile.print(tempo);
    myFile.print(bmp.readTemperature()); // graus celsius
    myFile.print(";");
    myFile.print(bmp.readPressure()); // pressão em Pa
    myFile.println();
    myFile.close();
    delay(60000);
  } else {
    digitalWrite(9, HIGH);
    delay(200);
    digitalWrite(9, LOW);
    delay(200);
    // LED pisca se arquivo nao abriu
  }
}