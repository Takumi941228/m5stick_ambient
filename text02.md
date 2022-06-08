# センサを使ったプログラミング

## ENV.Ⅲセンサを使った温湿度・気圧の取得

"""
SHT3X::SHT3X(uint8_t address)
{
  Wire.begin();
  #ifdef ARDUINO_M5Stick_C
    Wire.begin(0,26);
  #else
    Wire.begin();
  #endif
  _address=address;
}
"""