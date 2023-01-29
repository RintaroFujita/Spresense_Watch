#include <Wire.h>
#include <SpresenseNeoPixel.h>
volatile uint8_t data[14]; //センサからのデータ格納用配列
volatile int16_t az = 0;   //出力データ(生値)
const uint16_t PIN = 1;
const uint16_t NUM_PIXELS = 1;

int color[3];                     // RGBの値を格納する配列
float hue = 0.0;                  // 色相（0.0〜1.0）
float brightness = 0.0;           // 明るさ（0.0〜1.0）
SpresenseNeoPixel<PIN, NUM_PIXELS> neopixel;

void setup() {
  Serial.begin(115200);//シリアル通信を開始する  
  Wire.begin();//I2C通信を開始する
  Wire.beginTransmission(0x68);//送信処理を開始する(0x68がセンサーのアドレス)
  Wire.write(0x6b);            //レジスタ「0x6b」(動作状変数)を指定
  Wire.write(0x00);            //0x00を指定(ON)
  Wire.endTransmission();
  neopixel.framerate(60);    
}
void loop() {
  MPU_DATAGET();
  Serial.print("az: ");
  Serial.println(az);  
  delay(100);
  if (az <= -2000)
  {
    neopixel.set(0,200,0);
  }
  else if(az <= 2000)
  {
    neopixel.set(200,0,0);
  }
  else if(az <= 8000)
  {
    neopixel.set(0,0,200);
  }
  else 
  {
    neopixel.set(0,0,0);
  }
  neopixel.show();
}


void MPU_DATAGET() {
  Wire.beginTransmission(0x68); //送信処理を開始する
  Wire.write(0x3b);             //(取得値の先頭を指定)
  Wire.endTransmission();       //送信を終了する
  Wire.requestFrom(0x68, 14);   //データを要求する(0x3bから14バイトが6軸の値)

  uint8_t i = 0;
  while (Wire.available()) {
    data[i++] = Wire.read();
  }

  
  az = (data[0] << 8) | data[1];

}
