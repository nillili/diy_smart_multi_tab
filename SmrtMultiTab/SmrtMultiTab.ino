/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.


 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
SimpleTimer timer;

// 온습도 센서 init
#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).


//char auth[] = "토큰"; //홍상진
//char ssid[] = "neo";
//char pass[] = "053";


const int ledPin =  4;
const int RlyPin =  2;
int ledState   = LOW; 
int is_time    = 0;   // 타이머
int temp       = 0;   // 설정온도
int force_stop = 0;   // 강제
boolean to_do  = 0;   // 작동
int is_do      = 0;   // 작동 여부

int pinData;

BLYNK_WRITE(V1) //Button Widget is writing to pin V1
{
  pinData = param.asInt(); 
}

void setup()
{
  dht.begin();
  
  pinMode(ledPin, OUTPUT);
  pinMode(RlyPin, OUTPUT);
  
  // Debug console
  Serial.begin(9600);//시리얼 포트

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000L, sendSensor);
}

BLYNK_CONNECTED() 
{
  // Request Blynk server to re-send latest values for all pins
  Blynk.syncAll();
}

void loop()
{
  Blynk.run();
  timer.run();
}

/**
 * 타이머값 받기
 */
BLYNK_WRITE(V4) 
{
   is_time = param.asInt();
   Serial.print("Input Value from Server (Timer): ");
   Serial.println(is_time);
}

/**
 * 설정온도값 받기
 */
BLYNK_WRITE(V5) 
{
   temp = param.asInt();
   Serial.print("Input Value from Server (set temperature): ");
   Serial.println(temp);
}

/**
 * 강제
 */
BLYNK_WRITE(V7) 
{
   force_stop = param.asInt();
   Serial.print("Input Value from Server (force stop): ");
   Serial.println(force_stop);
}


float h = 0;           // 습도     
float t = 0;        // 온도

void sendSensor()
{

  /**
   * 온습도 읽기
   * 서버에 값을 전달
   */
  h = dht.readHumidity();           // 습도     
  t = dht.readTemperature();        // 온도

  if (isnan(h) || isnan(t)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
//  Serial.print(F("Humidity: "));
//  Serial.print(h);
//  Serial.print(F("%  Temperature: "));
//  Serial.print(t);
//  Serial.println(F("°C "));
//
  
  Blynk.virtualWrite(V2, t);
  Blynk.virtualWrite(V3, h);

  //////
  
  if(pinData == 1)
  {
    Serial.println("on");
    ledState = HIGH;
  }else
  {
    Serial.println("off");
    ledState = LOW;
  }
  digitalWrite(ledPin, ledState);

  /**
   * 조건 처리
   * 타이머     is_time
   * 설정온도   temp
   * 현재온도   t
   * 강제      force_stop
   * 작동      to_do
   */
  if(is_time == 1)
  {
    if(t >= temp)
    {
      to_do = true;
    }else
    {
      to_do = false;
    }
  }else
  {
    to_do = false; 
  }


  /**
   * 작동
   */
  if(force_stop == 1)
  {
    digitalWrite(RlyPin, !to_do);
  }else
  {
    digitalWrite(RlyPin, to_do);
  }

  /**
   * 채크
   */
  if(digitalRead(RlyPin) == 0)
  {
    Blynk.virtualWrite(V6, 0);
  }else
  {
    Blynk.virtualWrite(V6, 1);
  }
}
