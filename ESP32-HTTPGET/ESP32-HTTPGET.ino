#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Digitels";
const char* password = "Digitels123@!";
const char* serverName = "jsonplaceholder.typicode.com"; // Update the server URL
const int httpPort = 80; // Default HTTP port

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;  // Timer set to 5 seconds (5000 ms)

String dataRead;
float dataReadArr[3];

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Initiate HTTP request
  http.begin(client, serverName);

  // Send HTTP GET request and receive response
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Close HTTP connection
  http.end();

  return payload;
}

void connectWiFi(){
  int wifiTimerOut = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi" + String(ssid));
    Serial.print(".");
    wifiTimerOut++;
    if (wifiTimerOut > 20) {
      Serial.println("\nConnect WiFi fail!");
      return;
    }
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());
} 

void setup() {
  Serial.begin(115200);
  connectWiFi();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      // Make an HTTP GET request and receive response
      dataRead = httpGETRequest(serverName);
      Serial.println(dataRead);
      // Parse JSON response
      //JsonVariant myObject = JSON.parse(dataRead);
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, dataRead);
      
      if (error) {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      serializeJson(doc, Serial);
    
      // Extract and display data readings from JSON
      JsonObject root = doc.as<JsonObject>();
    
      for (JsonPair pair : root) {
        const char* key = pair.key().c_str();
        float value = pair.value().as<float>();
        
        if (strcmp(key, "1") == 0) {
          dataReadArr[0] = value;
        }
        else if (strcmp(key, "2") == 0) {
          dataReadArr[1] = value;
        }
        else if (strcmp(key, "3") == 0) {
          dataReadArr[2] = value;
        }
        
        Serial.print(key);
        Serial.print(" = ");
        Serial.println(value);
      }
      
      Serial.print("1 = ");
      Serial.println(dataReadArr[0]);
      Serial.print("2 = ");
      Serial.println(dataReadArr[1]);
      Serial.print("3 = ");
      Serial.println(dataReadArr[2]);
    }
    else {
        Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
