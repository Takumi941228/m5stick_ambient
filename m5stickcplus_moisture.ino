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