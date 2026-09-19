/////////////////////////////////
// B4 Bluetooth tracker — BT9
// Vocational high-school capstone, 2021
// TUNIOT for ESP32 (easycoding.tn) + hand edits
// Showcase note (2026): removed a duplicate volume()
// that sat between loop() and Pekora(); behaviour unchanged.
/////////////////////////////////
#include "BluetoothSerial.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "notes.h"//設定連結外部程式
//
String  DATA;
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
void printDeviceAddress() {

const uint8_t* point = esp_bt_dev_get_address();

for (int i = 0; i < 6; i++) {
   char str[3];
   sprintf(str, "%02X", (int)point[i]);
   Serial.print(str);
   if (i < 5){
   Serial.print(":");}
}
Serial.println();
}
//設定各種音效-------------------
int pekonote[]={G3,D4,G3,D4,G3,D4,F5,G5,F5,G3,D4,G3,D4,G3,D4,F5,G5,F5,
F5,D5,F5,F5,G5,F5,B5,G5,F5,F5,D5,F5,
G5,E5,G5,F5,G5,A5,B5,G5,F5,F5,D5,F5,
F5,D5,F5,F5,G5,F5,B5,G5,F5,F5,D5,F5,
G5,E5,G5,F5,G5,A5,B5,G5,F5,F5,D5,F5,
G5,E5,F5,G5,B5,A5,F5,G5,A5,C6,D6,C6,B5,A5,B5,F5,B5,A5,G5,F5,G5,D5,
E5,F5,G5,G5,A5,B5,C6,B5,A5,B5,C6,F5,B5,C6,D6,E6,F6,E6,D6,C6,G5,A5,B5,C6,D6,G5,D6,
G5,E5,F5,G5,B5,A5,F5,G5,A5,C6,D6,C6,B5,A5,B5,F5,B5,A5,G5,F5,G5,D5,
E5,F5,G5,G5,A5,B5,C6,B5,A5,B5,C6,A5,B5,C6,B5,F5,B5,B5,D6,B5,};//英文字母為音階，數字為音高。音效peko的音符
         
int pekoduration[]={4,4,4,4,4,4,11,11,11, 4,4,4,4,4,4,11,11,11,
2,2,2,11,11,11,2,2,2,11,11,11,
2,2,2,11,11,11,2,2,2,11,11,11,
2,2,2,11,11,11,2,2,2,11,11,11,
2,2,2,11,11,11,2,2,2,11,11,11,
10,10,10,10,3,10,10,10,10,3,10,10,10,10,4,4,10,10,10,10,4,4,
8,8,4,8,8,4,10,10,10,10,4,4, 10,10,10,10,10,10,10,10,10,10,10,10,8,8,4,
10,10,10,10,3,10,10,10,10,3,10,10,10,10,4,4,10,10,10,10,4,4,
8,8,4,8,8,4, 10,10,10,10,10,10,10,10,4,4,2,4,4,2,};//控制電流時長，數字越大時間越短。音效peko的節拍
int pekodutyCycle[]={300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
300,300,300,300,300,300,300,300,300,300,300,300,
300,300,300,300,300,300,300,300,300,300,300,300,
300,300,300,300,300,300,300,300,300,300,300,300,
300,300,300,300,300,300,300,300,300,300,300,300,
300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
300,300,300,300,300,300,300,300,300,300,300,300,300,300,400,300,400,300,400,300,}; //控制PWM脈寬，即可控制音量大小。脈寬越大、音量越大。音效peko的音量
int glnote[]={0,0,0,CS5,E5,
FS5,0,0,E5,FS5,0,0,E5,
FS5,A5,E5,FS5,CS5,0,CS5,E5,
FS5,0,0,E5,FS5,0,0,E5,
FS5,CS6,A5,B5,FS5,0,CS5,E5,
FS5,0,0,E5,FS5,0,0,E5,
FS5,A5,E5,FS5,CS5,E5,A4,B4,
CS5,A5,FS5,CS6,
B5,CS6,B5,A5,B5,FS5,0,
FS5,FS5,FS5,FS5,A5,B5,CS6,
FS5,FS5,FS5,E5,E5,FS5,
FS5,FS5,FS5,FS5,A5,B5,CS6,
FS5,FS5,FS5,D6,D6,CS6,
FS5,FS5,FS5,FS5,A5,B5,CS6,
FS5,FS5,FS5,E5,E5,FS5,
FS5,FS5,FS5,FS5,A5,B5,CS6,
FS6,E5,E5,FS5,FS5,};//英文字母為音階，數字為音高。音效gl的音符
         
int glduration[]={2,2,2,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
2,2,2,2,
4,8,8,4,4,2,2,
2,2,2,8,8,8,8,
2,2,4,4,4,4,
2,2,2,8,8,8,8,
2,2,4,4,4,4,
2,2,2,8,8,8,8,
2,2,4,4,4,4,
2,2,2,8,8,8,8,
2,4,4,4,2,//控制電流時長，數字越大時間越短。音效gl的節拍
}; 
int gldutyCycle[]={0,0,0,300,300,
300,0,0,300,300,0,0,300,
300,300,300,300,300,0,300,300,
300,0,0,300,300,0,0,300,
300,300,300,300,300,0,300,300,
300,0,0,300,300,0,0,300,
300,300,300,300,300,300,300 ,300,
300,300,300,300,
300,300,300,300,300,300,0,
300,300,300,300,300,300,300,
300,300,300,300,300,300,
300,300,300,300,300,300,300,
300,300,300,300,300,300,
300,300,300,300,300,300,300,
300,300,300,300,300,300,
300,300,300,300,300,300,300,
300,300,300,300,300,};//控制PWM脈寬，即可控制音量大小。脈寬越大、音量越大。音效gl的音量
int beenote[]={G5, E5, E5, 0, F5, D5, D5, 0, C5, D5, E5, F5, G5, G5, G5, 0,
            G5, E5, E5, 0, F5, D5, D5, 0, C5, E5, G5, G5, E5, 0, 0, 0,
            D5, D5, D5, D5, D5, E5, F5, 0, E5, E5, E5, E5, E5, F5, G5, 0,
            G5, E5, E5, 0, F5, D5, D5, 0, C5, E5, G5, G5, C5, 0, 0, 0};//英文字母為音階，數字為音高。音效bee的音符
         
int beeduration[]={4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};//控制電流時長，數字越大時間越短。音效bee的節拍
int beedutyCycle[]={300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300,
                300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300,
                300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300,
                300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300};//控制PWM脈寬，即可控制音量大小。脈寬越大、音量越大。音效bee的音量

int starnote[]={C5, C5, G5, G5, A5, A5, G5, 0, F5, F5, E5, E5, D5, D5, C5, 0,
                      G5, G5, F5, F5, E5, E5, D5, 0, G5, G5, F5, F5, E5, E5, D5, 0,
                      C5, C5, G5, G5, A5, A5, G5, 0, F5, F5, E5, E5, D5, D5, C5, 0};//英文字母為音階，數字為音高。音效star的音符
int starduration[]={4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                          4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                          4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};//控制電流時長，數字越大時間越短。音效star的節拍
int stardutyCycle[]={300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300,
                          300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300,
                          300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300};//控制PWM脈寬，即可控制音量大小。脈寬越大、音量越大。音效star的音量
int skynote[]={0,0,0,B4,CS5,D5,CS5,D5,F5,CS5,CS5,CS5,F4,F4,
 B4,A4,B4,D5,A4,A4,A4,F4,G4,F4,G4,D5,
 F4,F4,0,D5,D5,D5,CS5,G4,G4,CS5,CS5,CS5,0,B4,CS5,
 D5,CS5,D5,F5,CS5,CS5,CS5,F4,F4,B4,A4,B4,D5,
 A4,A4,A4,E4,F4,G4,D5,CS5,CS5,D5,D5,E5,E5,F5,D5,D5,D5,
 D5,CS5,B4,B4,CS5,A4,B4,B4,B4,D5,E5,F5,E5,F5,A5,
 E5,E5,E5,A4,A4,D5,CS5,D5,F5,F5,F5,F5,F5,
 B4,CS5,D5,CS5,E5,E5,D5,A4,A4,A4,G5,F5,E5,D5,
 F5,F5,F5,F5,B5,B5,A5,A5,F5,E5,D5,D5,0,D5,
 E5,D5,E5,E5,A5,F5,F5,F5,F5,B5,B5,A5,A5,
 F5,E5,D5,D5,0,D5,E5,D5,E5,E5,CS5,B4,B4,B4,B4,CS5};//英文字母為音階，數字為音高。音效sky的音符
         
int skyduration[]={4,4,4,2,2,2,2,4,4,4,4,4,2,2,
2,2,4,4,4,4,4,4,2,2,4,4,
4,4,2,2,2,2,2,2,4,4,4,4,4,2,2,
2,2,4,4,4,4,4,2,2,2,2,4,4,
4,4,4,2,2,4,2,6,6,6,2,2,2,2,6,2,4,
2,2,2,2,4,4,4,4,4,2,2,2,2,4,4,
4,4,4,2,2,2,2,4,4,4,4,4,4,
2,2,4,4,2,2,2,6,2,4,4,4,4,4,
4,4,4,4,4,4,4,4,2,2,4,4,2,2,
4,2,2,4,4,4,4,4,4,4,4,4,4,
2,2,4,4,2,2,4,2,6,2,4,4,4,4,2,2//控制電流時長，數字越大時間越短。音效sky的節拍
};
int skydutyCycle[]={0,0,0,300,300,300,300,300,300,300,300,300,300,300,
 300,300,300,300,300,300,300,300,300,300,300,300,
 300,300,0,300,300,300,300,300,300,300,300,300,0,300,300,
 300,300,300,300,300,300,300,300,300,300,300,300,300,
 300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
 300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,
 300,300,300,300,300,300,300,300,300,300,300,300,300,
 300,300,300,300,300,300,300,300,300,300,300,300,300,300,
 300,300,300,300,300,300,300,300,300,300,300,300,0,300,
 300,300,300,300,300,300,300,300,300,300,300,300,300,
 300,300,300,300,0,300,300,300,300,300,300,300,300,300,300,300};//控制PWM脈寬，即可控制音量大小。脈寬越大、音量越大。音效sky的音量
//預設各項數值-------------------
int stpnote=B4;
int stpduration=16;
int stpdutyCycle=0;
int buzzerPin=14;
int BUZZER_PIN = 14;     //蜂鳴器及喇叭控制腳位
int freq = 2000;
int Buzzer_Channel = 0;     //channel提供0-15共16個通道。
int resolution = 10;  //duty cycle解析度(2^10=1024解析度，變數dutyCycle最大值為1024)
 int GJvolume=300;
 int pekovolume=300;
 int beevolume=300;
 int starvolume=300;
 int skyvolume=300;//各音效的預設音量
 int stopvoid=0;
 int idost=0;
 const int BUTTON_PIN = 12;  // 按鍵的接腳
int buttonState = 0;//控制PWM脈寬，即可控制音量大小。脈寬越大、音量越大。
void setup()
{
DATA = "";

Serial.begin(115200);
pinMode(BUZZER_PIN, OUTPUT);
  ledcSetup(Buzzer_Channel, freq, resolution);  //設定通道0的頻率與duty cycle解析度。
  ledcAttachPin(BUZZER_PIN, Buzzer_Channel);    //控制蜂鳴器的腳位綁定於通道0
pinMode(22, OUTPUT);
  delay(5000);
  SerialBT.begin("追蹤器1");//ESP32wemoslolin32lite
  Serial.println("");
  printDeviceAddress();
pinMode(BUTTON_PIN, INPUT_PULLUP); //設定按鈕的接腳為輸入，因為我們要讀取它的狀態
}

int btdutyCycle=100;
//設定藍芽接收命令後執行動作-----------------
void loop()
{
buttonState = digitalRead(BUTTON_PIN);
    if (SerialBT.available()) {
      DATA = (SerialBT.readString());
      Serial.println("NEW DATA:");
      Serial.println(DATA);
      stopvoid=0;
      if (DATA == "關燈") {
        digitalWrite(22,HIGH);
      }
      if (DATA == "開燈") {
        digitalWrite(22,LOW);
      }
      if (DATA == "播放Pekora bgm") {
       Pekora(buzzerPin, sizeof(pekonote)/sizeof(int));
      }//如果收到訊息"播放Pekora bgm"就執行Pekora動作(放Peko音效)
      if (DATA == "播放極樂淨土") {
        GJ(buzzerPin, sizeof(glnote)/sizeof(int));
      }//如果收到訊息"播放極樂淨土"就執行GJ動作(放GJ音效)
      if (DATA == "播放小蜜蜂") {
        littleBee(buzzerPin, sizeof(beenote)/sizeof(int));
      }//如果收到訊息"播放小蜜蜂"就執行littleBee動作(放littleBee音效)
      if (DATA == "播放小星星") {
       star(buzzerPin, sizeof(starnote)/sizeof(int)); 
    }//如果收到訊息"播放小星星"就執行star動作(放star音效)
    if (DATA == "播放天空之城") {
       sky(buzzerPin, sizeof(skynote)/sizeof(int)); 
    }//如果收到訊息"播放天空之城"就執行sky動作(放sky音效)
      if (DATA == "停止播放") {
        stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        }//如果收到訊息"停止播放"就執行STP動作(停止音效音效)
      if (DATA == "vi6") {
        btdutyCycle=100;
         Serial.println("volume:");
      Serial.println(btdutyCycle);}
//設定收到訊息來改變音量------------       
    if (DATA == "vi5") {
        btdutyCycle=60;
        Serial.println("volume:");
      Serial.println(btdutyCycle);}
       if (DATA == "vi4") {
        btdutyCycle=20;
         Serial.println("volume:");
      Serial.println(btdutyCycle);}
      if (DATA == "vi3") {
        btdutyCycle=15;
         Serial.println("volume:");
      Serial.println(btdutyCycle);}
      if (DATA == "vi2") {
        btdutyCycle=10;
         Serial.println("volume:");
      Serial.println(btdutyCycle);}
      if (DATA == "vi1") {
        btdutyCycle=5;
         Serial.println("volume:");
      Serial.println(btdutyCycle);}
       if (DATA == "vi0") {
        btdutyCycle=0;
         Serial.println("volume:");
      Serial.println(btdutyCycle);}

}
}//-----------------------------
  //以子程式的方式將各種音效的動作存放在內
void Pekora(int pin, int count) {
 for (int i=0; i<count; i++) {
    int d=1000/pekoduration[i];
    ledcWriteTone(Buzzer_Channel, pekonote[i]);
    pekovolume=(pekodutyCycle[i]/100)*btdutyCycle;
    ledcWrite(Buzzer_Channel, pekovolume);
    int p=d * 2.08;
    delay(p);
    buttonState = digitalRead(BUTTON_PIN);
    if (SerialBT.available()) {
      DATA = (SerialBT.readString());
      Serial.println("NEW DATA:");
      Serial.println(DATA);
      idost=1;
      if (DATA == "停止播放") {
        stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
        Serial.println(stopvoid);
        break;
        }
       volume(buzzerPin, sizeof(stpnote)/sizeof(int));
       Serial.println("idost");
       Serial.println(idost);
       if (idost==1) {
        dost(buzzerPin, sizeof(glnote)/sizeof(int));
        stopvoid=1;
        break;
        }
      }
      if(buttonState == LOW){          //如果按鍵按了
     stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
        Serial.println(stopvoid);
        break;
  }
    }  
  if (stopvoid==0) {
        Pekora(buzzerPin, sizeof(glnote)/sizeof(int));}else{                           
        stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
  }//else{ 
       
  }
  
void GJ(int pin, int count) {
 for (int i=0; i<count; i++) {
    int d=1000/glduration[i];
    ledcWriteTone(Buzzer_Channel, glnote[i]);
    GJvolume=(gldutyCycle[i]/100)*btdutyCycle;
    ledcWrite(Buzzer_Channel,GJvolume );
    int p=d * 1.04;
    delay(p);
    buttonState = digitalRead(BUTTON_PIN);
    if (SerialBT.available()) {
      DATA = (SerialBT.readString());
      Serial.println("NEW DATA:");
      Serial.println(DATA);
      idost=1;
      if (DATA == "停止播放") {
        stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
        Serial.println(stopvoid);
        break;
        }
       volume(buzzerPin, sizeof(stpnote)/sizeof(int));
       Serial.println("idost");
       Serial.println(idost);
       if (idost==1) {
        dost(buzzerPin, sizeof(glnote)/sizeof(int));
        stopvoid=1;
        break;
        }
      }
      if(buttonState == LOW){          //如果按鍵按了
     stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
        Serial.println(stopvoid);
        break;
       
  }
      }       
    Serial.println("volume:");
      Serial.println(GJvolume);
      Serial.println(stopvoid);
      if (stopvoid==0) {
        GJ(buzzerPin, sizeof(glnote)/sizeof(int));}else{                           
        stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
  }
        
  }
void littleBee(int pin, int count) {
 for (int i=0; i<count; i++) {
    int d=1000/beeduration[i];
    ledcWriteTone(Buzzer_Channel, beenote[i]);
    beevolume=(beedutyCycle[i]/100)*btdutyCycle;
    ledcWrite(Buzzer_Channel, beevolume);
    int p=d * 1.3;
    delay(p);
    buttonState = digitalRead(BUTTON_PIN);
    if (SerialBT.available()) {
      DATA = (SerialBT.readString());
      Serial.println("NEW DATA:");
      Serial.println(DATA);
      idost=1;
      if (DATA == "停止播放") {
        stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
        Serial.println(stopvoid);
        break;
        }
       volume(buzzerPin, sizeof(stpnote)/sizeof(int));
       Serial.println("idost");
       Serial.println(idost);
       if (idost==1) {
        dost(buzzerPin, sizeof(glnote)/sizeof(int));
        stopvoid=1;
        break;
        }
      }
      if(buttonState == LOW){          //如果按鍵按了
     stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
        Serial.println(stopvoid);
        break;
  }
  }
  if (stopvoid==0) {
        littleBee(buzzerPin, sizeof(glnote)/sizeof(int));}else{                           
        stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
  }
  }
  void star(int pin, int count) {
  for (int i=0; i<count; i++) {
    int d=1000/starduration[i];
    ledcWriteTone(Buzzer_Channel, starnote[i]);
    starvolume=(stardutyCycle[i]/100)*btdutyCycle;
    ledcWrite(Buzzer_Channel, starvolume);
    int p=d * 1.3;
    delay(p);
    buttonState = digitalRead(BUTTON_PIN);
    if (SerialBT.available()) {
      DATA = (SerialBT.readString());
      Serial.println("NEW DATA:");
      Serial.println(DATA);
      idost=1;
      if (DATA == "停止播放") {
        stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
        Serial.println(stopvoid);
        break;
        }
       volume(buzzerPin, sizeof(stpnote)/sizeof(int));
       Serial.println("idost");
       Serial.println(idost);
       if (idost==1) {
        dost(buzzerPin, sizeof(glnote)/sizeof(int));
        stopvoid=1;
        break;
        }
      }
      if(buttonState == LOW){          //如果按鍵按了
     stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
        Serial.println(stopvoid);
        break;
  }
    }
    if (stopvoid==0) {
        star(buzzerPin, sizeof(glnote)/sizeof(int));
    }else{                           
        stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
  }
    }
    void sky(int pin, int count) {
  for (int i=0; i<count; i++) {
    int d=1000/skyduration[i];
    ledcWriteTone(Buzzer_Channel, skynote[i]);
    skyvolume=(skydutyCycle[i]/100)*btdutyCycle;
    ledcWrite(Buzzer_Channel, skyvolume);
    int p=d * 1.5;
    delay(p);
    buttonState = digitalRead(BUTTON_PIN);
    if (SerialBT.available()) {
      DATA = (SerialBT.readString());
      Serial.println("NEW DATA:");
      Serial.println(DATA);
      idost=1;
      if (DATA == "停止播放") {
        stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
        Serial.println(stopvoid);
        break;
        }
       volume(buzzerPin, sizeof(stpnote)/sizeof(int));
       Serial.println("idost");
       Serial.println(idost);
       if (idost==1) {
        dost(buzzerPin, sizeof(glnote)/sizeof(int));
        stopvoid=1;
        break;
        }
      }
      if(buttonState == LOW){          //如果按鍵按了
     stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
        Serial.println(stopvoid);
        break;
  }
    }
    if (stopvoid==0) {
        sky(buzzerPin, sizeof(glnote)/sizeof(int));
    }else{                           
        stp(buzzerPin, sizeof(stpnote)/sizeof(int));
        stopvoid=1;
  }
    }
  void stp(int pin, int count) {
 for (int i=0; i<count; i++) {
    int d=1000/stpduration;
    ledcWriteTone(Buzzer_Channel, stpnote);
    ledcWrite(Buzzer_Channel, stpdutyCycle);
    int p=d * 0.5;
    delay(p);//停止音效用子程式
    }
  }
   void volume(int pin, int count) {
 for (int i=0; i<count; i++) {
      if (DATA == "vi6") {
        btdutyCycle=100;
        idost=0;
         Serial.println("volume:");
      Serial.println(btdutyCycle);} 
    if (DATA == "vi5") {
        btdutyCycle=60;
        idost=0;
        Serial.println("volume:");
      Serial.println(btdutyCycle);}
       if (DATA == "vi4") {
        btdutyCycle=20;
        idost=0;
         Serial.println("volume:");
      Serial.println(btdutyCycle);}
      if (DATA == "vi3") {
        btdutyCycle=15;
        idost=0;
         Serial.println("volume:");
      Serial.println(btdutyCycle);}
      if (DATA == "vi2") {
        btdutyCycle=10;
        idost=0;
         Serial.println("volume:");
      Serial.println(btdutyCycle);}
      if (DATA == "vi1") {
        btdutyCycle=5;
        idost=0;
         Serial.println("volume:");
      Serial.println(btdutyCycle);}
       if (DATA == "vi0") {
        btdutyCycle=0;
        idost=0;
         Serial.println("volume:");
      Serial.println(btdutyCycle);}
      if (DATA == "關燈") {
        digitalWrite(22,HIGH);
        idost=0;
      }
      if (DATA == "開燈") {
        digitalWrite(22,LOW);
        idost=0;
      }
    }
  }//聲音調整子程式
  void dost(int pin, int count) {
 for (int i=0; i<count; i++) {
    if (DATA == "播放Pekora bgm") {
       Pekora(buzzerPin, sizeof(pekonote)/sizeof(int));
      }
      if (DATA == "播放極樂淨土") {
        GJ(buzzerPin, sizeof(glnote)/sizeof(int));
      }
      if (DATA == "播放小蜜蜂") {
        littleBee(buzzerPin, sizeof(beenote)/sizeof(int));
      }
      if (DATA == "播放小星星") {
       star(buzzerPin, sizeof(starnote)/sizeof(int)); 
    }
    if (DATA == "播放天空之城") {
       sky(buzzerPin, sizeof(skynote)/sizeof(int)); 
    }  
    }//換音效子程式，設定若接收到新的命令可換音效
  }
