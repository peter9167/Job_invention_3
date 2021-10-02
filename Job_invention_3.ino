#include <TimerOne.h> // TimeOne라이브러리 사용
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include <DHT.h> // DHT라이브러리 사용
#define DHTPIN 4 //핀 번호 설정
#define DHTTYPE DHT11 // DHT 사용 번호 설정 (DHT11)
DHT dht(DHTPIN, DHTTYPE); // DHT 설정

LiquidCrystal_I2C lcd(0x27, 16, 2);

//-------타이머 인터럽트를 위한 타이머 카운트------------
int time_count = 0;
int temp_count = 0;

extern volatile unsigned long timer0_millis; //타이머변수
unsigned long timeVal; //이전시간
unsigned long readTime; //현재타이머시간
int hour, min, sec;
boolean state=false;

int btnPin = 2;

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  Serial.println("DHT11 Start..");
  
  dht.begin();

  pinMode(btnPin, INPUT);
}

void loop() {
  int temp = dht.readTemperature(); //온도 값 저장 변수
  long t = millis() / 1000;
  int btn = digitalRead(btnPin);

  Serial.print("Temperature : ");
  Serial.println(temp);
  
  lcd.setCursor(0,0);
  lcd.print("Temperature : ");
  lcd.print(temp);
  if(btn == 1){
    String inString = Serial.readStringUntil('\n');    
    int index1 = inString.indexOf(':'); 
    int index2 = inString.indexOf(':',index1+1);   
    int index3 = inString.length();
    
    hour = inString.substring(0, index1).toInt();
    min = inString.substring(index1+1,index2).toInt();
    sec = inString.substring(index2+1,index3).toInt();
  
    
    timer0_millis = ((long)hour*3600+min*60+sec)*1000;
    state=true;
    timeVal=millis();
  } 
  if(state==true){ //시리얼모니털 출력 시작
    
    if(millis()-timeVal>=1000){ //1초 단위 출력
     readTime = millis()/1000;
      
     if(millis()>=86400000){
       timer0_millis=0;
     }
     timeVal = millis();
   
     sec = readTime%60;
     min = (readTime/60)%60;
     hour = (readTime/(60*60))%24;       

     lcd.setCursor(0,1);
     lcd.print(hour);
     lcd.print(" : ");
     lcd.print(min);
     lcd.print(" : ");
     lcd.print(sec);      
    }
  }
}
