// Communication and Networking Libraries
#include <esp_now.h>          // ESP-NOW communication protocol
#include <WiFi.h>             // ESP32 WiFi functionality
#include <WiFiUdp.h>          // UDP (User Datagram Protocol) functionality for WiFi

// Timekeeping and NTP Libraries
#include <ESP32Time.h>        // Library for timekeeping on ESP32
#include <NTPClient.h>        // NTP (Network Time Protocol) client library
#include <RTClib.h>           // Library for working with real-time clocks

// Core Libraries
#include <Arduino.h>          // Arduino core library
#include <Preferences.h>      // Library for storing data in non-volatile memory

ESP32Time rtc;

const char* ntpServer = "pool.ntp.org";    // NTP server to fetch time from
const long gmtOffset_sec = 25200;         // Time zone: GMT+7 (in seconds)

WiFiUDP ntpUDP; // UDP object for NTP
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec);

//for testing send ESP-NOW|ESP32 Sender Sketch (ESP-NOW)
// REPLACE WITH YOUR RECEIVER MAC Address
//E0:5A:1B:AA:D8:6C
uint8_t broadcastAddress[] = {0xE0, 0x5A, 0x1B, 0xAA, 0xD8, 0x6C};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void disconnectWiFi() {
  WiFi.disconnect(true, true); // Memutus koneksi WiFi dan menghapus pengaturan jaringan yang telah tersimpan
}
// Fungsi untuk menghubungkan ke WiFi
void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin("Digitels Device", "Digitels123@!");
  int wifiTimerOut = 0; // Correcting the variable name here
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    wifiTimerOut++; // Increment the timeout counter
    if (wifiTimerOut > 20) {
      Serial.println("\nConnect WiFi fail!");
      return;
    }
  }
  Serial.println("\nConnected to WiFi");
}


void getTimeFromInternet() {
  timeClient.begin();
  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *tmTime = localtime((const time_t *)&epochTime);

  int day = tmTime->tm_mday;
  int month = tmTime->tm_mon + 1; // tm_mon is 0-based, so add 1 to get the actual month
  int year = tmTime->tm_year + 1900; // tm_year is years since 1900
  int hour = tmTime->tm_hour;
  int minute = tmTime->tm_min;
  int second = tmTime->tm_sec;

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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //connectToWiFi
  connectToWiFi();
  // Set the current time of the real-time clock (RTC)
  getTimeFromInternet();
  
  //disconect  internet
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

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  getTimeFromInternet();

  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = false;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
