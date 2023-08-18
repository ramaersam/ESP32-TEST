#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Digitels";
const char* password = "Digitels123@!";
const char* server = "jsonplaceholder.typicode.com"; // Update the server URL
const int httpPort = 80; // Default HTTP port


void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  WiFiClient client;
  
  if (!client.connect(server, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  
  // Make an HTTP GET request
  client.print("GET /posts/1/comments HTTP/1.1\r\n");
  client.print("Host: ");
  client.print(server);
  client.print("\r\n");
  client.print("Connection: close\r\n\r\n");
  
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }
  
  // Read and print the response data
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
  
  Serial.println("Response received");
  
  client.stop();
  
  // Wait for a period of time before making the next request
  delay(5000);
}
