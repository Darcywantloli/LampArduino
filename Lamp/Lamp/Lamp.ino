#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
// 宣告藍牙變數(Tx, Rx)
SoftwareSerial HM10(2, 3);
// 宣告WS2812變數(燈泡數, pin)
Adafruit_NeoPixel pixels(16, 6);
// 宣告變數 vr(可變電阻)
const byte vr = A0;
// 宣告變數 bluetoothOnOff(藍牙狀態), lampOnOff(檯燈狀態)
bool bluetoothOnOff = false;
bool lampOnOff = false;
// 藍牙特徵值 
char appData;
String appString = "";
// 亮度
int ledBright = 0;

void setup() {
  // 13：藍牙開關, 12：檯燈開關
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  // 7：藍牙狀態燈
  pinMode(7, OUTPUT);
  // 啟動藍牙及監控視窗
  HM10.begin(9600); 
  Serial.begin(9600);
  // 啟動WS2812及清除狀態
  pixels.begin();
  pixels.clear();
}

void loop() {
  HM10.listen();
  HM10.setTimeout(100);
  
  bluetooth();
  lamp();
}

void lamp() {
  if (digitalRead(12) == LOW) {
    lampOnOff = !lampOnOff;
    Serial.println("Lamp,switch");
    delay(1000);
  }
  if (lampOnOff == false) {
    setLight(0);
  } else {
    if (bluetoothOnOff == false) {
      int vrValue = analogRead(vr);
      vrValue = ((vrValue / 4) - 90) * 1.5;
      setLight(vrValue);
    } else {
      setLight(ledBright);
    }
  }
}

void bluetooth() {
  if (digitalRead(13) == LOW) {
    bluetoothOnOff = !bluetoothOnOff;
    Serial.println("Bluetooth,switch");
    delay(1000);
  }
  if (bluetoothOnOff == false) {
    digitalWrite(7, LOW);
  } else {
    digitalWrite(7, HIGH);
  }
  if (HM10.available()) {
    appData = HM10.read();
    appString = String(appData);
    if (appString == "A") {
      lampOnOff = false;
      ledBright = 0;
      Serial.println("A");
    }
    if (appString == "B") {
      lampOnOff = true;
      ledBright = 255;
      Serial.println("B");
    }
    if (appString == "C") {
      bluetoothOnOff = true;
      lampOnOff = true;
      ledBright = 255;
      Serial.println("C");
    }
    if (appString == "0") {
      ledBright = 1;
      Serial.println("0");
    }
    if (appString == "1") {
      ledBright = 51;
      Serial.println("1");
    }
    if (appString == "2") {
      ledBright = 102;
      Serial.println("2");
    }
    if (appString == "3") {
      ledBright = 153;
      Serial.println("3");
    }
    if (appString == "4") {
      ledBright = 204;
      Serial.println("4");
    }
    if (appString == "5") {
      ledBright = 255;
      Serial.println("5");
    }
  }
}

void setLight(int bright) {
  for(int i=0; i<16; i++) { //設定「每一顆」燈的顏色
  	pixels.setPixelColor(i, pixels.Color(255, 255, 255)); //設定燈的顏色
  }
  pixels.setBrightness(bright);
  pixels.show();
}
