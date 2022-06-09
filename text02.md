# 各種センサを使ったプログラミング

## ENV.Ⅲセンサを使った温湿度・気圧の取得

ライブラリからENV.ⅢセンサのI2Cバス対応のpin(0,26)に変更するため、使用しないところはコメントアウトする。

 - [libraries] → [src] → [SHT3X.cpp]

```shell
SHT3X::SHT3X(uint8_t address)
{
  #Wire.begin();
  #ifdef ARDUINO_M5Stick_C
    Wire.begin(0,26);
  #else
    #Wire.begin();
  #endif
  _address=address;
}
```
ENV.Ⅲ HATを取り付け、以下のプログラムを書き込む。

```
/* m5stickcplus_env3.ino */

#include <M5StickCPlus.h>
#include "UNIT_ENV.h"

SHT3X sht30;
QMP6988 qmp6988;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

void setup() {
  M5.begin(); //Init M5Stack.
  M5.Axp.ScreenBreath(8); //バックライト0~12
  M5.Lcd.setRotation(3);
  M5.lcd.setTextSize(2);  //Set the text size to 2. 
  Wire.begin(); //Wire init, adding the I2C bus.
  qmp6988.init();
  M5.lcd.println(F("ENV Unit III test"));
}

void loop() {
  pressure = qmp6988.calcPressure();
  if(sht30.get()==0){ //Obtain the data of shT30.
    tmp = sht30.cTemp;  //Store the temperature obtained from shT30.
    hum = sht30.humidity; //Store the humidity obtained from the SHT30.
  }else{
    tmp=0,hum=0;
  }
  M5.lcd.fillRect(0,20,100,60,BLACK); //Fill the screen with black (to clear the screen).
  M5.lcd.setCursor(0,20);
  M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa\r\n", tmp, hum, pressure);
  delay(2000);
}
```
土壌水分センサを取り付け、以下のプログラムを書き込む。

| 名称 | 色 | pin |
| --- | --- | --- |  
| VCC | 赤 | Vout |
| GND | 黒 | GND |
| Vout | 黄色 | G33 |

```
/* m5stickcplus_moisture.ino */

#include <M5StickCPlus.h>

#define ADC_PIN 33

int potential = 0;

const int ADC_MAX = 2880; // 空気中の実測値
const int ADC_MIN = 1400; // 水に浸した実測値

void setup() {
  pinMode(ADC_PIN, INPUT);
  Serial.begin(115200);
  M5.begin();
  M5.Axp.ScreenBreath(8); //バックライト0~12
  M5.Lcd.setRotation(3);
  M5.lcd.setTextSize(2);  //Set the text size to 2.  设置文字大小为2
  M5.lcd.println(F("Moisture Sensor test"));
}

void loop() {
  potential = analogRead(ADC_PIN);
  Serial.println(potential);
  if (ADC_MAX < potential) {
    potential = ADC_MAX;
  }
  else if (ADC_MIN > potential) {
    potential = ADC_MIN;
  }

  potential = 100 - map(potential, ADC_MIN, ADC_MAX, 0, 100);
  M5.lcd.fillRect(0, 20, 100, 60, BLACK); //Fill the screen with black (to clear the screen).  将屏幕填充黑色(用来清屏)
  M5.lcd.setCursor(0, 20);
  M5.Lcd.print(potential);
  delay(2000);

}
```