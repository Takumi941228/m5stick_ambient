#include <M5StickCPlus.h>
#include "UNIT_ENV.h"
#include "Ambient.h"

#define ADC_PIN 33
#define led 10

const char* ssid = "IT-IoT";
const char* password = "Passw0rd";
const int channelId = 51171;
const char* writeKey = "ff5e60199524f0f3";

WiFiClient client;
Ambient ambient;
SHT3X sht30;
QMP6988 qmp6988;

int potential = 0;
float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

const int ADC_MAX = 2880; // 空気中の実測値
const int ADC_MIN = 1400; // 水に浸した実測値

void setup() {
  Serial.println("WiFi connecting....");
  WiFi.begin(ssid, password); //  Wi-Fiの初期化
  while (WiFi.status() != WL_CONNECTED) {  //  Wi-Fiアクセスポイントへの接続待ち
    delay(500);
  }
  Serial.println("WiFi connected!!!");
  ambient.begin(channelId, writeKey, &client);

  pinMode(ADC_PIN, INPUT);
  pinMode(led, OUTPUT);

  Serial.begin(115200);
  M5.begin();
  M5.Axp.ScreenBreath(8); //バックライト0~12
  M5.Lcd.setRotation(3);
  M5.lcd.setTextSize(2);  //Set the text size to 2.  设置文字大小为2
  Wire.begin(); //Wire init, adding the I2C bus.  Wire初始化, 加入i2c总线
  qmp6988.init();
  M5.lcd.println(F("Plant Ficus"));
}

int counter = 0;

void loop() {
  pressure = qmp6988.calcPressure();
  if (sht30.get() == 0) { //Obtain the data of shT30.  获取sht30的数据
    tmp = sht30.cTemp - 10.0;  //Store the temperature obtained from shT30.  将sht30获取到的温度存储
    hum = sht30.humidity; //Store the humidity obtained from the SHT30.  将sht30获取到的湿度存储
  } else {
    tmp = 0, hum = 0;
  }
  potential = analogRead(ADC_PIN);
  Serial.println(potential);
  if (ADC_MAX < potential) {
    potential = ADC_MAX;
  }
  else if (ADC_MIN > potential) {
    potential = ADC_MIN;
  }

  potential = 100 - map(potential, ADC_MIN, ADC_MAX, 0, 100);
  if (potential < 60) {
    digitalWrite(led, 1);
  }
  else if (potential >= 60) {
    digitalWrite(led, 0);
  }
  Serial.println(potential);
  M5.lcd.fillRect(0, 20, 100, 60, BLACK); //Fill the screen with black (to clear the screen).  将屏幕填充黑色(用来清屏)
  M5.lcd.setCursor(0, 20);
  M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa \r\nMoist:%2d%%", tmp, hum, pressure, potential);
  sendAmbient(tmp, hum, pressure, potential);
  delay(1000 * 30);
}

void sendAmbient(float temp, float humid, float puress, int moist) {
  ambient.set(1, temp);
  ambient.set(2, humid);
  ambient.set(3, puress);
  ambient.set(4, moist);
  ambient.send();
}