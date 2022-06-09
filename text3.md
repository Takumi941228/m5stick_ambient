# Ambientを使ってセンサ値をWeb上で可視化する

M5Stickを用いてセンサ値を実際の日常または業務的に活用しようとすると、グラフによる可視化したくなるケースが大いにあると感じられます。時間による変化や過去のあると特定の値と比較することで、初めて見えてくる気付きもあると思います。

今回はAmbeintという無料で誰でも数値を可視化できるインターネットのサービスを使って、M5Stick-C-Plusから温湿度・気圧・土壌水分量をインターネット経由で送信し、折れ線グラフで見る方法を行います。具体的には、Ambient用のライブラリを利用し、取得したセンサ値をインターネット経由で定期的にAmbientに送信し、Ambientは送られてきたセンサ値を自動的に蓄積していくので、ユーザはWebアプリケーションとして、蓄積されたデータを折れ線グラフとして確認ができます。

## Ambient用ライブラリのインストール

Arduino IDEのメニュー[スケッチ] → [ライブラリをインクルード] → [ライブラリを管理]でライブラリマネージャーを開き、検索バーに「ambient」と入力し、「Ambient ESP32 ESP8266 lib」をInstall allでインストールする。

## プログラムを書き込む

```
#include <M5StickCPlus.h>
#include "UNIT_ENV.h"
#include "Ambient.h"

#define ADC_PIN 33
#define led 10

const char* ssid = "IT-IoT";
const char* password = "Passw0rd";
const int channelId = 51121;
const char* writeKey = "646a96c43a8b625d";

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
  M5.lcd.setTextSize(2);  //Set the text size to 2.
  Wire.begin(); //Wire init, adding the I2C bus.
  qmp6988.init();
  M5.lcd.println(F("Plant Ficus"));
}

int counter = 0;

void loop() {
  pressure = qmp6988.calcPressure();
  if (sht30.get() == 0) { //Obtain the data of shT30.
    tmp = sht30.cTemp - 10.0;  //Store the temperature obtained from shT30.
    hum = sht30.humidity; //Store the humidity obtained from the SHT30.
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
  if(potential < 5){
    digitalWrite(led, 1);
  }
  else if(potential >= 5){
    digitalWrite(led, 0);
  }
  M5.lcd.fillRect(0, 20, 100, 60, BLACK); //Fill the screen with black (to clear the screen).
  M5.lcd.setCursor(0, 20);
  M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa \r\nMoist:%2d%%", tmp, hum, pressure, potential);

  if (counter % 6 == 0) {
    sendAmbient(tmp, hum, pressure, potential);
  }
  counter++;
  delay(2000);
}

void sendAmbient(float temp, float humid, float puress, int moist) {
  ambient.set(1, temp);
  ambient.set(2, humid);
  ambient.set(3, puress);
  ambient.set(4, moist);
  ambient.send();
}
```