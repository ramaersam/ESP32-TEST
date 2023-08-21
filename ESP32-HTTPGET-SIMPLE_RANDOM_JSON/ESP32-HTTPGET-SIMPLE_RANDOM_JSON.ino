#include <WiFi.h>
#include <WiFiClientSecure.h> // Import WiFiClientSecure library
#include <HTTPClient.h>

const char* ssid = "Digitels";
const char* password = "Digitels123@!";

const char* serverName = "devs.digitels.me";
const char* requestPath = "/irdata?TV=polytron&slot=1";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds, it will take 5 seconds before making the first request.");
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      String responseData = httpGETRequest(serverName, requestPath);
      Serial.println("Received data: ");
      Serial.println(responseData);
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* server, const char* path) {
  WiFiClientSecure client; // Use WiFiClientSecure instead of WiFiClient
  HTTPClient http;

  // Additional setup for secure connection
  client.setInsecure(); // Set connection as insecure (not recommended for production)

  String fullUrl = String("https://") + String(server) + String(path);
  http.begin(client, fullUrl);

  int httpResponseCode = http.GET();
  String responsePayload = "";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    responsePayload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
  return responsePayload;
}
