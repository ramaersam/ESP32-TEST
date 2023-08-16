#include <esp_now.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "driver/gpio.h"
#include <ESP32Time.h>
#include <NTPClient.h>
#include <Preferences.h>

ESP32Time rtc;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 25200;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec);

int day, month, year, hour, minute, second;
unsigned long epochTime;  // Declaration for epoch time
struct tm *tmTime;         // Declaration for time structure
const char* dayNames[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

void disconnectWiFi() {
  WiFi.disconnect(true, true);
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin("Digitels Device", "Digitels123@!");
  int wifiTimerOut = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    wifiTimerOut++;
    if (wifiTimerOut > 20) {
      Serial.println("\nConnect WiFi fail!");
      return;
    }
  }
  Serial.println("\nConnected to WiFi");
}

void getTimeFromInternet() {
  timeClient.update();
  timeClient.begin();
  epochTime = timeClient.getEpochTime();
  tmTime = localtime((const time_t *)&epochTime);
  day = tmTime->tm_mday;
  month = tmTime->tm_mon + 1;
  year = tmTime->tm_year + 1900;
  hour = tmTime->tm_hour;
  minute = tmTime->tm_min;
  second = tmTime->tm_sec;

  rtc.setTime(hour, minute, second, day, month, year);
  // Debugging (optional)
  Serial.print("Date: ");
  Serial.print(day);
  Serial.print("-");
  Serial.print(month);
  Serial.print("-");
  Serial.println(year);
  Serial.print("Time: ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);
}


void sendTimeToLVGL() {
  rtc.setTime(hour, minute, second, day, month, year);
  const char* dayName = dayNames[tmTime->tm_wday];
  String daynameStr = String(dayName);
  
  String dayStr = String(day, DEC);
  if (day < 10) {
    dayStr = "0" + dayStr;
  }
  
  // Assuming rtc.getTime() returns a String
  String dateStr = daynameStr + String(", ") +  dayStr  + rtc.getTime(" %B") + rtc.getTime(" %Y");
  
  Serial.println(dateStr);
  Serial.print(hour);
  Serial.print(":");
  Serial.println(minute);

  Serial2.printf("t0.txt=\"%s\"\xff\xff\xff", dateStr.c_str());
  Serial2.printf("n3.val=%d\xff\xff\xffn4.val=%d\xff\xff\xff", hour, minute);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  rtc.setTime(hour, minute, second, day, month, year);

  //connectToWiFi
  connectToWiFi();
  // Set the current time of the real-time clock (RTC)
  getTimeFromInternet();
  sendTimeToLVGL();
  //disconect internet
  disconnectWiFi();
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else{
    Serial.println("OK initializing ESP-NOW");
  }


}

void loop() {
  getTimeFromInternet();
  sendTimeToLVGL();


  delay(2000);
}
