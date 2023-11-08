#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
// 宣告藍牙變數(Tx, Rx)
SoftwareSerial HM10(2, 3);
// 宣告WS2812變數(燈泡數, pin)
Adafruit_NeoPixel pixels(16, 6);
// 宣告變數 Vr(可變電阻)
const byte Vr = A0;
// 宣告變數 Bluetooth(藍牙狀態), LampOnOff(檯燈狀態)
bool Bluetooth = false;
bool LampOnOff = false;
// 藍牙特徵值 
char appData;
String appString = "";
// 亮度
int LEDBright = 0;

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

  if (HM10.available()) {
    appData = HM10.read();
    appString = String(appData);
    if (appString == "C") {
      LampOnOff = true;
      Bluetooth = true;
      LEDBright = 255;
      Serial.println("Off");
    }
  }

  if (digitalRead(13) == LOW) {
    Bluetooth = !Bluetooth;
    Serial.println("Bluetooth,OK");
    delay(1000);
  }
  
  if (Bluetooth == false) {
    digitalWrite(7, LOW);
    lamp();
  } else {
    digitalWrite(7, HIGH);
    bluetooth();
  }

  if (LampOnOff == false) {
    pixels.setBrightness(0);
    pixels.show();
  } else {
    if (Bluetooth == false) {
      int VrValue = analogRead(Vr);
      VrValue = ((VrValue / 4) - 90) * 1.5;
      setLight(VrValue);
    } else {
      setLight(LEDBright);
    }
  }
}

void lamp() {
  if (digitalRead(12) == LOW) {
    LampOnOff = !LampOnOff;
    Serial.println("Lamp,OK");
    delay(1000);
  }
}

void setLight(int bright) {
  for(int i=0; i<16; i++) { //設定「每一顆」燈的顏色
  	pixels.setPixelColor(i, pixels.Color(255, 255, 255)); //設定燈的顏色
  }
  pixels.setBrightness(bright);
  pixels.show();
}

void bluetooth() {
  if (HM10.available()) {
    appData = HM10.read();
    appString = String(appData);
    if (appString == "A") {
      LampOnOff = false;
      LEDBright = 0;
      Serial.println("Off");
    }
    if (appString == "B") {
      LampOnOff = true;
      LEDBright = 255;
      Serial.println("On");
    }
    if (appString == "0") {
      LEDBright = 0;
      Serial.println("0");
    }
    if (appString == "1") {
      LEDBright = 51;
      Serial.println("1");
    }
    if (appString == "2") {
      LEDBright = 102;
      Serial.println("2");
    }
    if (appString == "3") {
      LEDBright = 153;
      Serial.println("3");
    }
    if (appString == "4") {
      LEDBright = 204;
      Serial.println("4");
    }
    if (appString == "5") {
      LEDBright = 255;
      Serial.println("5");
    }
  }
}
