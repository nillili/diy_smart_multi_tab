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


//char auth[] = "0wB6ZPg2_r53NPFNKs8L7xA4KpjgsjFu"; //이선우
char auth[] = "3CmeaeOhhrnMFTHUKQVw0iO5eRarQFcl"; //홍상진
//char auth[] = "MN6w88J069vMz0WSRZyY1co_BpPGHg7x"; //장정권


// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "jjangsvc";
//char pass[] = "123456789a";
char ssid[] = "neozio";
char pass[] = "0536259956";

const int ledPin =  4;
int ledState = LOW; 


int pinData;

BLYNK_WRITE(V1) //Button Widget is writing to pin V1
{
  pinData = param.asInt(); 
}

void setup()
{
  dht.begin();
  
  pinMode(ledPin, OUTPUT);
  // Debug console
  Serial.begin(9600);//시리얼 포트

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}


void sendSensor()
{

  // 온습도 읽기

  float h = dht.readHumidity();           // 습도     
  float t = dht.readTemperature();        // 온도

  if (isnan(h) || isnan(t)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  
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
}
