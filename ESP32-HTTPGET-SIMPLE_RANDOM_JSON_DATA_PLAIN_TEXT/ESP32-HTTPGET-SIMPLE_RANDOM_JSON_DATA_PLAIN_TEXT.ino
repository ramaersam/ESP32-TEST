#include <WiFi.h>
#include <WiFiClientSecure.h> // Import WiFiClientSecure library
#include <HTTPClient.h>
#include <SPIFFS.h>

const char* ssid = "Digitels Device";
const char* password = "Digitels123@!";

const char* serverName = "devs.digitels.me";
const char* requestPath = "/irdata?TV=polytron&slot=1";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

bool requestData = false; // Flag to indicate when to trigger data request

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

void saveToFile(const char* fileName, const String& data) {
  File file = SPIFFS.open(fileName, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  if (file.print(data)) {
    Serial.println("Data written to file");
  } else {
    Serial.println("Write failed");
  }

  file.close();
}

String readFromFile(const char* fileName) {
  File file = SPIFFS.open(fileName, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return "";
  }

  String data = file.readString();
  file.close();
  return data;
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to " + String(ssid));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }

  Serial.println("Timer set to 5 seconds, it will take 5 seconds before making the first request.");
  Serial.println("Type 'GETDATA' and press Enter to trigger data request.");
  
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
     if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim(); // Remove leading and trailing whitespaces
  
      if (input.equalsIgnoreCase("GETDATA")) {
        requestData = true;
      }
    }   
    
  if (requestData) {
    requestData = false; // Reset the flag
        
    if (WiFi.status() == WL_CONNECTED) {
      // Get data from the server
      String responseData = httpGETRequest(serverName, requestPath);
      Serial.println("Received data: ");
      Serial.println(responseData);
      
      // Save the data to a file
      saveToFile("/data.txt", responseData); 
           
      // Read and display saved data from the file
      String savedData = readFromFile("/data.txt");
      Serial.println("Saved data: ");
      Serial.println(savedData);      
      
    } else {
      Serial.println("WiFi Disconnected");
    }
  } 
    lastTime = millis();
    delay(5000); // Delay for 5 seconds before making the next request
  }
}
